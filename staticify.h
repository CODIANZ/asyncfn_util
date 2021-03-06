#if !defined(__h_staticify__)
#define __h_staticify__

#include <tuple>
#include <functional>

namespace asyncfn_util {

template <int N, typename FUNCTION> class staticify;

template <int N, typename RESULT, typename ...ARGS>
  class staticify<N, RESULT (*) (ARGS...)>
{
public:
  using F_RES   = RESULT;
  using F_ARGS  = std::tuple<ARGS...>;
  using F_FUNC  = std::function<RESULT(ARGS...)>;
  using F_TYPE  = RESULT(ARGS...);

private:
  struct wrap_t {
    F_FUNC f;
  };

  F_FUNC  m_func;

  static F_RES sfunc(ARGS ...args){
    auto params = std::tuple<ARGS...>(args...);
    auto w = reinterpret_cast<wrap_t*>(std::get<N>(params));
    auto f = w->f;
    delete w;
    return f(args...);
  } 

public:
  staticify(F_FUNC f): m_func(f) {}
  void* refcon() { return new wrap_t{ .f = m_func }; }
  static F_TYPE* callback() { return &sfunc; }
};

} /* using asyncfn_util */

#endif /* __h_staticify__ */