#if !defined(__h_ppack__)
#define __h_ppack__

#include <tuple>

namespace asyncfn_util {

template <std::size_t N, std::size_t I, typename ...ARGS> struct pick_impl;
template <std::size_t N, std::size_t I, typename T0, typename T1, typename ...Tx>
  struct pick_impl<N, I, T0, T1, Tx...> :
    std::conditional<
      N == I,
      pick_impl<N, I + 1, T0>,
      pick_impl<N, I + 1, T1, Tx...>
    >::type {};
template <std::size_t N, std::size_t I, typename T>
  struct pick_impl<N, I, T>{ using type = T; };

template <typename ...T> struct bundle;

template <std::size_t START, std::size_t END, std::size_t I, template<typename ...> class TARGET, typename ...T>
  struct range_forward_impl;
template <std::size_t START, std::size_t END, std::size_t I, template<typename ...> class TARGET, typename ...TARGS, typename T0, typename ...Tx>
  struct range_forward_impl<START, END, I, TARGET, bundle<TARGS...>, T0, Tx...> :
    std::conditional<
      (START <= I) && (I <= END),
      range_forward_impl<START, END, I + 1, TARGET, bundle<TARGS..., T0>, Tx...>,
      range_forward_impl<START, END, I + 1, TARGET, bundle<TARGS...>, Tx...>
    >::type {};
template <std::size_t START, std::size_t END, std::size_t I, template<typename ...> class TARGET, typename ...TARGS>
  struct range_forward_impl<START, END, I, TARGET, bundle<TARGS...>>
{
  using type = TARGET<TARGS...>;
};

template <typename ...T> struct ppack {
  static constexpr std::size_t length = sizeof...(T);

  template <std::size_t N>
    struct get_s
  {
    static_assert(N >= 0);
    static_assert(N < length);
    using type = typename pick_impl<N, 0, T...>::type;
  };
  
  template <std::size_t N> using get = typename get_s<N>::type;

  template <template<typename ...> class TARGET> using forward = TARGET<T...>;

  template <std::size_t N, std::size_t C, template<typename ...> class TARGET>
    struct range_forward_s
  {
    static_assert(N >= 0);
    static_assert(C > 0);
    static_assert(N + C <= length);
    using type = typename range_forward_impl<N, N + C - 1, 0, TARGET, bundle<>, T...>::type;
  };

  template <std::size_t N, std::size_t C, template<typename ...> class TARGET>
    using range_forward = typename range_forward_s<N, C, TARGET>::type;
};

} /* namespace asyncfn_util */

#endif /* !defined(__h_ppack__) */