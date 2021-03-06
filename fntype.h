#if !defined(__h_fntype__)
#define __h_fntype__

#include <tuple>
#include <functional>

namespace asyncfn_util {

template <typename T> struct fntype;

template <typename R, typename ...ARGS> struct fntype<R(*)(ARGS...)>
{
  using type = R(ARGS...);
};

template <typename R, typename ...ARGS> struct fntype<R(ARGS...)>
{
  using type = R(ARGS...);
};

template <typename R, typename ...ARGS> struct fntype<R(&)(ARGS...)>
{
  using type = R(ARGS...);
};

} /* using asyncfn_util */

#endif /* __h_fntype__ */