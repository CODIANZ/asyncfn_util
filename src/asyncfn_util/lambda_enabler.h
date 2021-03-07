#if !defined(__h_lambda_enabler__)
#define __h_lambda_enabler__

#include <tuple>
#include <functional>
#include <type_traits>

#include "staticify.h"
#include "fntype.h"
#include "pickargs.h"
#include "function_traits.h"

namespace asyncfn_util {

enum class rcorder { rc, cr };

template <rcorder RC_ORDER, refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  class basic_lambda_enabler
{
protected:
  using F_RES       = RESULT;
  using F_ARGS      = std::tuple<ARGS...>;
  using F_FUNC      = std::function<RESULT(ARGS...)>;

  static constexpr auto F_ARGS_COUNT = std::tuple_size<F_ARGS>::value;

  static constexpr auto N_REFCON_POS    = RC_ORDER == rcorder::rc ? 1 : 0;
  static constexpr auto N_CALLBACK_POS  = RC_ORDER == rcorder::rc ? 0 : 1;

  using REFCON_TYPE = typename std::tuple_element<F_ARGS_COUNT - N_REFCON_POS - 1, F_ARGS>::type;

  using CB_F_TYPE   = typename std::tuple_element<F_ARGS_COUNT - N_CALLBACK_POS - 1, F_ARGS>::type;
  using CB_RES      = typename function_traits<CB_F_TYPE>::result;
  using CB_ARGS     = typename function_traits<CB_F_TYPE>::arguments;

  using STATICIFY   = staticify<REFCON_POS, CB_F_TYPE>;
  using CB_FUNC     = std::function<typename fntype<CB_F_TYPE>::type>;

  static_assert(std::is_pointer<REFCON_TYPE>::value == true);
  static_assert(std::is_function<typename fntype<CB_F_TYPE>::type>::value == true);

  using OP_F_ARGS   = typename pick<F_ARGS_COUNT - 2, ARGS...>::types;

private:
  const F_FUNC m_target;

protected:
  F_FUNC target() const { return m_target; }

public:
  basic_lambda_enabler(F_FUNC target) : m_target(target) {}

  template <typename ...LARGS> auto prepare(LARGS&& ...largs) const {
    using P1 = typename std::tuple_element<F_ARGS_COUNT - 2, F_ARGS>::type;
    using P2 = typename std::tuple_element<F_ARGS_COUNT - 1, F_ARGS>::type;
    struct p {
      const std::function <F_RES(P1, P2)> f;
      void call(CB_FUNC callback) {
        STATICIFY sfn(callback);
        /* dirty code ... */
        auto void_callback  = (void*)(sfn.callback());
        auto void_refcon    = (void*)sfn.refcon();
        if(RC_ORDER == rcorder::rc){
          f(reinterpret_cast<P1>(void_refcon), reinterpret_cast<P2>(void_callback));
        }
        else{
          f(reinterpret_cast<P1>(void_callback), reinterpret_cast<P2>(void_refcon));
        }
      }
    };
    return p{ .f = std::bind(target(), largs..., std::placeholders::_1, std::placeholders::_2)};
  }

#if __cplusplus >= 201703L
  void operator () (OP_F_ARGS args, CB_FUNC callback) const {
    using P1 = typename std::tuple_element<F_ARGS_COUNT - 2, F_ARGS>::type;
    using P2 = typename std::tuple_element<F_ARGS_COUNT - 1, F_ARGS>::type;
    STATICIFY sfn(callback);
    /* dirty code ... */
    auto void_callback  = (void*)(sfn.callback());
    auto void_refcon    = (void*)sfn.refcon();
    auto refcon_callback = std::tuple<P1, P2>(
      reinterpret_cast<P1>(
        RC_ORDER == rcorder::rc ? void_refcon : void_callback
      ),
      reinterpret_cast<P2>(
        RC_ORDER == rcorder::rc ? void_callback : void_refcon
      )
    );
    auto params = std::tuple_cat(args, refcon_callback);
    std::apply(target(), params);
  }
#endif
};

template <rcorder RC_ORDER, refconpos REFCON_POS, typename FUNCTION> class lambda_enabler;

template <rcorder RC_ORDER, refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  struct lambda_enabler<RC_ORDER, REFCON_POS, RESULT(ARGS...)> : public basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>
{
  lambda_enabler(typename basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>::F_FUNC target) :
    basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>(target) {}
};

template <rcorder RC_ORDER, refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  struct lambda_enabler<RC_ORDER, REFCON_POS, RESULT(*)(ARGS...)> : public basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>
{
  lambda_enabler(typename basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>::F_FUNC target) :
    basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>(target) {}
};

template <rcorder RC_ORDER, refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  struct lambda_enabler<RC_ORDER, REFCON_POS, RESULT(&)(ARGS...)> : public basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>
{
  lambda_enabler(typename basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>::F_FUNC target) :
    basic_lambda_enabler<RC_ORDER, REFCON_POS, RESULT, ARGS...>(target) {}
};

} /* namespace asyncfn_util */

#endif /* !defined(__h_lambda_enabler__) */