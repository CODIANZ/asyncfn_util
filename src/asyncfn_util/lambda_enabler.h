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

template <int N, typename RESULT, typename ...ARGS>
  class basic_lambda_enabler
{
protected:
  using F_RES       = RESULT;
  using F_ARGS      = std::tuple<ARGS...>;
  using F_FUNC      = std::function<RESULT(ARGS...)>;

  static constexpr auto F_ARGS_COUNT = std::tuple_size<F_ARGS>::value;

  using REFCON_TYPE = typename std::tuple_element<F_ARGS_COUNT - 2, F_ARGS>::type;

  using CB_F_TYPE   = typename std::tuple_element<F_ARGS_COUNT - 1, F_ARGS>::type;
  using CB_RES      = typename function_traits<CB_F_TYPE>::result;
  using CB_ARGS     = typename function_traits<CB_F_TYPE>::arguments;

  using STATICIFY   = staticify<N, CB_F_TYPE>;
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
  template <typename ...LARGS> auto prepare(LARGS ...largs) const {
    struct p {
      const std::function <F_RES(void*, CB_F_TYPE)> f;
      void call(CB_FUNC callback) {
        STATICIFY sfn(callback);
        f(sfn.refcon(), sfn.callback());
      }
    };
    return p{ .f = std::bind(target(), largs..., std::placeholders::_1, std::placeholders::_2)};
  }

#if __cplusplus >= 201703L
  void operator () (OP_F_ARGS args, CB_FUNC callback) const {
    STATICIFY sfn(callback);
    auto refcon_callback = std::tuple<REFCON_TYPE, CB_F_TYPE>(reinterpret_cast<REFCON_TYPE>(sfn.refcon()), sfn.callback());
    auto params = std::tuple_cat(args, refcon_callback);
    std::apply(target(), params);
  }
#endif
};

template <int N, typename FUNCTION> class lambda_enabler;

template <int N, typename RESULT, typename ...ARGS>
  struct lambda_enabler<N, RESULT(ARGS...)> : public basic_lambda_enabler<N, RESULT, ARGS...>
{
  lambda_enabler(typename basic_lambda_enabler<N, RESULT, ARGS...>::F_FUNC target) :
    basic_lambda_enabler<N, RESULT, ARGS...>(target) {}
};

template <int N, typename RESULT, typename ...ARGS>
  struct lambda_enabler<N, RESULT(*)(ARGS...)> : public basic_lambda_enabler<N, RESULT, ARGS...>
{
  lambda_enabler(typename basic_lambda_enabler<N, RESULT, ARGS...>::F_FUNC target) :
    basic_lambda_enabler<N, RESULT, ARGS...>(target) {}
};

template <int N, typename RESULT, typename ...ARGS>
  struct lambda_enabler<N, RESULT(&)(ARGS...)> : public basic_lambda_enabler<N, RESULT, ARGS...>
{
  lambda_enabler(typename basic_lambda_enabler<N, RESULT, ARGS...>::F_FUNC target) :
    basic_lambda_enabler<N, RESULT, ARGS...>(target) {}
};

} /* namespace asyncfn_util */

#endif /* !defined(__h_lambda_enabler__) */