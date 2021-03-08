#if !defined(__h_pickargs__)
#define __h_pickargs__

#include <tuple>

namespace asyncfn_util {

template <std::size_t, std::size_t, typename ...> struct pickargs_impl;

/* I < N */
template <std::size_t N, std::size_t I, typename ...FARGS, typename T, typename ...ARGS>
  struct pickargs_impl<N, I, std::tuple<FARGS...>, std::tuple<>, T, ARGS...> :
    pickargs_impl<N, I + 1, std::tuple<FARGS..., T>, std::tuple<>, ARGS...> {};

/* I > N */
template <std::size_t N, std::size_t I, typename ...FARGS, typename BT, typename ...BARGS, typename T, typename ...ARGS>
  struct pickargs_impl<N, I, std::tuple<FARGS...>, std::tuple<BT, BARGS...>, T, ARGS...> :
    pickargs_impl<N, I + 1, std::tuple<FARGS...>, std::tuple<BT, BARGS..., T>, ARGS...> {};

/* I == N */
template <std::size_t N, typename ...FARGS, typename T, typename ...ARGS>
  struct pickargs_impl<N, N, std::tuple<FARGS...>, std::tuple<>, T, ARGS...> :
    pickargs_impl<N, N + 1, std::tuple<FARGS...>, std::tuple<T>, ARGS...> {};
  
/* last */
template <std::size_t N, std::size_t I, typename ...FARGS, typename ...BARGS>
  struct pickargs_impl<N, I, std::tuple<FARGS...>, std::tuple<BARGS...>>
{
  using type  = std::tuple<FARGS...>;
  using after = std::tuple<BARGS...>;
};

template <std::size_t N, typename ...ARGS>
  struct pickargs :
    pickargs_impl<N, 0, std::tuple<>, std::tuple<>, ARGS...> {};

template <int N, typename ...ARGS> struct pickargs_r {
  using before  = typename pickargs<std::tuple_size<std::tuple<ARGS...>>::value - N, ARGS...>::type;
  using type    = typename pickargs<std::tuple_size<std::tuple<ARGS...>>::value - N, ARGS...>::after;
};

} /* namespace asyncfn_util */

#endif /* !defined(__h_pickargs__) */