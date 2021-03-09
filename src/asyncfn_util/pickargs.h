#if !defined(__h_pickargs__)
#define __h_pickargs__

#include <tuple>

namespace asyncfn_util {

template <std::size_t N, std::size_t I, typename ...T> struct pickargs_impl;

template <std::size_t N, std::size_t I, typename ...FRONT, typename T, typename ...ARGS>
  struct pickargs_impl<N, I, std::tuple<FRONT...>, std::tuple<>, T, ARGS...> :
    std::conditional<
      I == N,
      pickargs_impl<N, I + 1, std::tuple<FRONT...>, std::tuple<T, ARGS...>>,
      pickargs_impl<N, I + 1, std::tuple<FRONT..., T>, std::tuple<>, ARGS...>
    >::type {};

template <std::size_t N, std::size_t I, typename ...FRONT, typename ...BACK>
  struct pickargs_impl<N, I, std::tuple<FRONT...>, std::tuple<BACK...>>
{
  using front = typename std::tuple<FRONT...>;
  using back  = typename std::tuple<BACK...>;
};

template <std::size_t N, typename ...ARGS> struct pickargs
{
  static_assert(N > 0, "N <= 0");
  static_assert(N <= std::tuple_size<std::tuple<ARGS...>>::value, "N > ARGS");
  using type = typename pickargs_impl<N, 0, std::tuple<>, std::tuple<>, ARGS...>::front;
};

template <std::size_t N, typename ...ARGS> struct pickargs_r
{
  static_assert(N > 0, "N <= 0");
  static_assert(N <= std::tuple_size<std::tuple<ARGS...>>::value, "N > ARGS");
  using type = typename pickargs_impl<std::tuple_size<std::tuple<ARGS...>>::value - N, 0, std::tuple<>, std::tuple<>, ARGS...>::back;
};

} /* namespace asyncfn_util */

#endif /* !defined(__h_pickargs__) */