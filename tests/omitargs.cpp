#include <iostream>
#include <asyncfn_util/omitargs.h>

using namespace asyncfn_util;

void test_omitargs() {
  std::cout << "test_omitargs()" << std::endl;
  
  using T0 = int;
  using T1 = float;
  using T2 = double;
  using T3 = int*;

  /* omitargs */

  static_assert(std::is_same<omitargs<0>::type, std::tuple<>>::value);

  static_assert(std::is_same<omitargs<0, T0>::type, std::tuple<>>::value);

  static_assert(std::is_same<omitargs<0, T0, T1>::type, std::tuple<T1>>::value);
  static_assert(std::is_same<omitargs<1, T0, T1>::type, std::tuple<T0>>::value);

  static_assert(std::is_same<omitargs<0, T0, T1, T2>::type, std::tuple<T1, T2>>::value);
  static_assert(std::is_same<omitargs<1, T0, T1, T2>::type, std::tuple<T0, T2>>::value);
  static_assert(std::is_same<omitargs<2, T0, T1, T2>::type, std::tuple<T0, T1>>::value);

  static_assert(std::is_same<omitargs<0, T0, T1, T2, T3>::type, std::tuple<T1, T2, T3>>::value);
  static_assert(std::is_same<omitargs<1, T0, T1, T2, T3>::type, std::tuple<T0, T2, T3>>::value);
  static_assert(std::is_same<omitargs<2, T0, T1, T2, T3>::type, std::tuple<T0, T1, T3>>::value);
  static_assert(std::is_same<omitargs<3, T0, T1, T2, T3>::type, std::tuple<T0, T1, T2>>::value);

  /* omitargs_r */

  static_assert(std::is_same<omitargs_r<0>::type, std::tuple<>>::value);

  static_assert(std::is_same<omitargs_r<0, T0>::type, std::tuple<>>::value);

  static_assert(std::is_same<omitargs_r<0, T0, T1>::type, std::tuple<T0>>::value);
  static_assert(std::is_same<omitargs_r<1, T0, T1>::type, std::tuple<T1>>::value);

  static_assert(std::is_same<omitargs_r<0, T0, T1, T2>::type, std::tuple<T0, T1>>::value);
  static_assert(std::is_same<omitargs_r<1, T0, T1, T2>::type, std::tuple<T0, T2>>::value);
  static_assert(std::is_same<omitargs_r<2, T0, T1, T2>::type, std::tuple<T1, T2>>::value);

  static_assert(std::is_same<omitargs_r<0, T0, T1, T2, T3>::type, std::tuple<T0, T1, T2>>::value);
  static_assert(std::is_same<omitargs_r<1, T0, T1, T2, T3>::type, std::tuple<T0, T1, T3>>::value);
  static_assert(std::is_same<omitargs_r<2, T0, T1, T2, T3>::type, std::tuple<T0, T2, T3>>::value);
  static_assert(std::is_same<omitargs_r<3, T0, T1, T2, T3>::type, std::tuple<T1, T2, T3>>::value);
}
