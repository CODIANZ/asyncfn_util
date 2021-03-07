#if !defined(__h_staticify__)
#define __h_staticify__

#include <tuple>
#include <functional>

namespace asyncfn_util {

enum class refconpos { first, last };

template <refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  class basic_staticify
{
protected:
  using F_RES   = RESULT;
  using F_ARGS  = std::tuple<ARGS...>;
  using F_FUNC  = std::function<RESULT(ARGS...)>;
  using F_TYPE  = RESULT(ARGS...);
  static constexpr auto F_ARGS_COUNT = std::tuple_size<F_ARGS>::value;
  static constexpr auto REFCON_INDEX = REFCON_POS == refconpos::first ? 0 : F_ARGS_COUNT -1;

private:
  struct wrap_t {
    F_FUNC f;
  };

  F_FUNC  m_func;

  static F_RES sfunc(ARGS ...args){
    auto params = std::tuple<ARGS...>(args...);
    auto refcon = std::get<REFCON_INDEX>(params);
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

template <refconpos REFCON_POS, typename T> class staticify;

template<refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  struct staticify<REFCON_POS, RESULT(ARGS...)> : public basic_staticify<REFCON_POS, RESULT, ARGS...>
{
  staticify(typename basic_staticify<REFCON_POS, RESULT, ARGS...>::F_FUNC f) :
    basic_staticify<REFCON_POS, RESULT, ARGS...>(f) {}
};

template<refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  struct staticify<REFCON_POS, RESULT(*)(ARGS...)> : public basic_staticify<REFCON_POS, RESULT, ARGS...>
{
  staticify(typename basic_staticify<REFCON_POS, RESULT, ARGS...>::F_FUNC f) :
    basic_staticify<REFCON_POS, RESULT, ARGS...>(f) {}
};

template<refconpos REFCON_POS, typename RESULT, typename ...ARGS>
  struct staticify<REFCON_POS, RESULT(&)(ARGS...)> : public basic_staticify<REFCON_POS, RESULT, ARGS...>
{
  staticify(typename basic_staticify<REFCON_POS, RESULT, ARGS...>::F_FUNC f) :
    basic_staticify<REFCON_POS, RESULT, ARGS...>(f) {}
};

} /* using asyncfn_util */

#endif /* __h_staticify__ */