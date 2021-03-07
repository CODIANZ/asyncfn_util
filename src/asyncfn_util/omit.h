#if !defined(__h_omit__)
#define __h_omit__

#include <tuple>

namespace asyncfn_util {

template <int N, typename ...ARGS> struct omit;

template <typename ...ARGS> struct omit_0 {
  using type = std::tuple<ARGS...>;
};
template <typename ...ARGS> struct omit<0, ARGS...> : public omit_0<ARGS...> {};

template <typename T1, typename ...ARGS> struct omit_1 {
  using type = std::tuple<ARGS...>;
};
template <typename ...ARGS> struct omit<1, ARGS...> : public omit_1<ARGS...> {};

#define __(V, TN, N) template <TN, typename ...ARGS> struct omit_##V { \
  using type = std::tuple<N, ARGS...>; \
}; \
template <typename ...ARGS> struct omit<V, ARGS...> : public omit_##V<ARGS...> {};
#define _(...) __VA_ARGS__

__( 2, _(typename T1, typename T2), _(T1))
__( 3, _(typename T1, typename T2, typename T3), _(T1, T2))
__( 4, _(typename T1, typename T2, typename T3, typename T4), _(T1, T2, T3))
__( 5, _(typename T1, typename T2, typename T3, typename T4, typename T5), _(T1, T2, T3, T4))
__( 6, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6), _(T1, T2, T3, T4, T5))
__( 7, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7), _(T1, T2, T3, T4, T5, T6))
__( 8, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8), _(T1, T2, T3, T4, T5, T6, T7))
__( 9, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9), _(T1, T2, T3, T4, T5, T6, T7, T8))
__(10, _(typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10), _(T1, T2, T3, T4, T5, T6, T7, T8, T9))

#undef _
#undef __

template <int N, typename ...ARGS> struct romit {
  using type = typename omit<std::tuple_size<std::tuple<ARGS...>>::value - N, ARGS...>::type;
};


} /* using asyncfn_util */

#endif /* __h_omit__ */