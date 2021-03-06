#if !defined(__h_observablify__)
#define __h_observablify__

#include <rxcpp/rx.hpp>
#include "lambda_enabler.h"

namespace asyncfn_util {

template <int N, typename RESULT, typename ...ARGS>
  class basic_observablify : public basic_lambda_enabler<N, RESULT, ARGS...>
{
  using SUPER = basic_lambda_enabler<N, RESULT, ARGS...>;
public:
  basic_observablify(typename SUPER::F_FUNC target) :
    basic_lambda_enabler<N, RESULT, ARGS...>(target) {}

  template <typename ...LARGS> auto rx(LARGS&& ...largs) const {
    return rxcpp::observable<>::create<typename SUPER::CB_ARGS>([=](rxcpp::subscriber<typename SUPER::CB_ARGS> s){
      this->prepare(largs...)
      .call([=](auto&& ...results){
        s.on_next(typename SUPER::CB_ARGS{results...});
        s.on_completed();
      });
    });
  }

#if __cplusplus >= 201703L
  auto rx(typename SUPER::OP_F_ARGS args) const {
    return rxcpp::observable<>::create<typename SUPER::CB_ARGS>([=](rxcpp::subscriber<typename SUPER::CB_ARGS> s){
      typename SUPER::STATICIFY sfn([=](auto&& ...results){
        s.on_next(typename SUPER::CB_ARGS{results...});
        s.on_completed();
      });
      auto refcon_callback = std::tuple<typename SUPER::REFCON_TYPE, typename SUPER::CB_F_TYPE>(reinterpret_cast<typename SUPER::REFCON_TYPE>(sfn.refcon()), sfn.callback());
      auto params = std::tuple_cat(args, refcon_callback);
      std::apply(this->target(), params);
    });
  }
#endif
};

template <int N, typename FUNCTION> class observablify;

template <int N, typename RESULT, typename ...ARGS>
  struct observablify<N, RESULT(ARGS...)> : public basic_observablify<N, RESULT, ARGS...>
{
  observablify(typename basic_observablify<N, RESULT, ARGS...>::F_FUNC target) :
    basic_observablify<N, RESULT, ARGS...>(target) {}
};

template <int N, typename RESULT, typename ...ARGS>
  struct observablify<N, RESULT(*)(ARGS...)> : public basic_observablify<N, RESULT, ARGS...>
{
  observablify(typename basic_lambda_enabler<N, RESULT, ARGS...>::F_FUNC target) :
    basic_observablify<N, RESULT, ARGS...>(target) {}
};

template <int N, typename RESULT, typename ...ARGS>
  struct observablify<N, RESULT(&)(ARGS...)> : public basic_observablify<N, RESULT, ARGS...>
{
  observablify(typename basic_lambda_enabler<N, RESULT, ARGS...>::F_FUNC target) :
    basic_observablify<N, RESULT, ARGS...>(target) {}
};

} /* using asyncfn_util */

#endif /* __h_observablify__ */