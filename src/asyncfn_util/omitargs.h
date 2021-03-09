#if !defined(__h_omit__)
#define __h_omit__

#include <tuple>

namespace asyncfn_util {

template<std::size_t, std::size_t, typename ...> struct omitargs_impl;

template <std::size_t N, std::size_t I, typename ...TPARGS, typename T, typename ...ARGS>
  struct omitargs_impl<N, I, std::tuple<TPARGS...>, T, ARGS...> :
    std::conditional<
      I == N,
      omitargs_impl<N, I + 1, std::tuple<TPARGS...>, ARGS...>,
      omitargs_impl<N, I + 1, std::tuple<TPARGS..., T>, ARGS...>
    >::type {};

template <std::size_t N, std::size_t I,typename ...TPARGS>
  struct omitargs_impl<N, I, std::tuple<TPARGS...>>
{
  using type = std::tuple<TPARGS...>;
};

template <std::size_t N, typename ...ARGS> struct omitargs {
  static_assert(N >= 0, "N < 0");
  static_assert(N < std::tuple_size<std::tuple<ARGS...>>::value, "N >= ARGS");
  using type = typename omitargs_impl<N, 0, std::tuple<>, ARGS...>::type;
};

template <std::size_t N, typename ...ARGS> struct omitargs_r {
  static_assert(N >= 0, "N < 0");
  static_assert(N < std::tuple_size<std::tuple<ARGS...>>::value, "N >= ARGS");
  using type = typename omitargs<std::tuple_size<std::tuple<ARGS...>>::value - N - 1, ARGS...>::type;
};

} /* using asyncfn_util */

#endif /* __h_omit__ */