#if !defined(__h_function_traits__)
#define __h_function_traits__

#include <tuple>

namespace asyncfn_util {

template<typename FUNCTION> struct function_traits;

template<typename RESULT, typename ...ARGS>
  struct function_traits<RESULT (ARGS...)>
{
  using result = RESULT;
  using arguments = std::tuple<ARGS...>;
};

template<typename RESULT, typename ...ARGS>
  struct function_traits<RESULT (&)(ARGS...)>
{
  using result = RESULT;
  using arguments = std::tuple<ARGS...>;
};

template<typename RESULT, typename ...ARGS>
  struct function_traits<RESULT (*)(ARGS...)>
{
  using result = RESULT;
  using arguments = std::tuple<ARGS...>;
};

} /* namespace asyncfn_util */

#endif /* !defined(__h_function_traits__) */