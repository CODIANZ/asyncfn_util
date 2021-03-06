#if !defined(__h_staticify__)
#define __h_staticify__

#include <tuple>
#include <functional>

namespace asyncfn_util {

template <int N, typename RESULT, typename ...ARGS>
  class basic_staticify
{
protected:
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
    auto refcon = std::get<N>(params);
    auto f = refconToWrap(refcon)->f;
    discardRefcon(refcon);
    return f(args...);
  } 

  static wrap_t* refconToWrap(void* refcon){
    return reinterpret_cast<wrap_t*>(refcon);
  }

public:
  basic_staticify(F_FUNC f): m_func(f) {}
  void* refcon() const { return new wrap_t{ .f = m_func }; }
  static F_TYPE* callback() { return &sfunc; }

  static void discardRefcon(void* refcon) {
    delete reinterpret_cast<wrap_t*>(refcon);
  }
};

template <int N, typename T> class staticify;

template<int N, typename RESULT, typename ...ARGS>
  struct staticify<N, RESULT(ARGS...)> : public basic_staticify<N, RESULT, ARGS...>
{
  staticify(typename basic_staticify<N, RESULT, ARGS...>::F_FUNC f) :
    basic_staticify<N, RESULT, ARGS...>(f) {}
};

template<int N, typename RESULT, typename ...ARGS>
  struct staticify<N, RESULT(*)(ARGS...)> : public basic_staticify<N, RESULT, ARGS...>
{
  staticify(typename basic_staticify<N, RESULT, ARGS...>::F_FUNC f) :
    basic_staticify<N, RESULT, ARGS...>(f) {}
};

template<int N, typename RESULT, typename ...ARGS>
  struct staticify<N, RESULT(&)(ARGS...)> : public basic_staticify<N, RESULT, ARGS...>
{
  staticify(typename basic_staticify<N, RESULT, ARGS...>::F_FUNC f) :
    basic_staticify<N, RESULT, ARGS...>(f) {}
};

} /* using asyncfn_util */

#endif /* __h_staticify__ */