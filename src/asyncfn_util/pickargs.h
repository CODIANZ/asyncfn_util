#if !defined(__h_pickargs__)
#define __h_pickargs__

#include <tuple>

namespace asyncfn_util {

template <int N, typename ...ARGS> struct pick;

template <typename ...ARGS> struct pick<0, ARGS...> {
  using types = std::tuple<>;
  using after = std::tuple<ARGS...>;
};

#define __(N, TN, T) \
template <TN, typename ...ARGS> struct pick<N, T, ARGS...> { \
  using types = std::tuple<T>; \
  using after = std::tuple<ARGS...>;\
};

#define _(...) __VA_ARGS__

__( 1, _(typename T1), _(T1))
__( 2, _(typename T1, typename T2), _(T1, T2))
__( 3, _(typename T1, typename T2, typename T3), _(T1, T2, T3))
__( 4, _(typename T1, typename T2, typename T3, typename T4), _(T1, T2, T3, T4))
__( 5, _(typename T1, typename T2, typename T3, typename T4, typename T5), _(T1, T2, T3, T4, T5))
__( 6, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6), _(T1, T2, T3, T4, T5, T6))
__( 7, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7), _(T1, T2, T3, T4, T5, T6, T7))
__( 8, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8), _(T1, T2, T3, T4, T5, T6, T7, T8))
__( 9, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9), _(T1, T2, T3, T4, T5, T6, T7, T8, T9))
__(10, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10), _(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))

#undef __
#undef _

template <int N, typename ...ARGS> struct rpick {
  using before  = typename pick<std::tuple_size<std::tuple<ARGS...>>::value - N, ARGS...>::types;
  using types   = typename pick<std::tuple_size<std::tuple<ARGS...>>::value - N, ARGS...>::after;
};

} /* namespace asyncfn_util */

#endif /* !defined(__h_pickargs__) */