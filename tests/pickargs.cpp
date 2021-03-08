#include <iostream>
#include <asyncfn_util/pickargs.h>

using namespace asyncfn_util;

void test_pickargs() {
  std::cout << "test_omitargs()" << std::endl;

  using T0 = int;
  using T1 = float;
  using T2 = double;
  using T3 = int*;

  /* pickargs */

  static_assert(std::is_same<pickargs<0>::type, std::tuple<>>::value);

  static_assert(std::is_same<pickargs<0, T0>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs<1, T0>::type, std::tuple<T0>>::value);

  static_assert(std::is_same<pickargs<0, T0, T1>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs<1, T0, T1>::type, std::tuple<T0>>::value);
  static_assert(std::is_same<pickargs<2, T0, T1>::type, std::tuple<T0, T1>>::value);

  static_assert(std::is_same<pickargs<0, T0, T1, T2>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs<1, T0, T1, T2>::type, std::tuple<T0>>::value);
  static_assert(std::is_same<pickargs<2, T0, T1, T2>::type, std::tuple<T0, T1>>::value);
  static_assert(std::is_same<pickargs<3, T0, T1, T2>::type, std::tuple<T0, T1, T2>>::value);

  static_assert(std::is_same<pickargs<0, T0, T1, T2, T3>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs<1, T0, T1, T2, T3>::type, std::tuple<T0>>::value);
  static_assert(std::is_same<pickargs<2, T0, T1, T2, T3>::type, std::tuple<T0, T1>>::value);
  static_assert(std::is_same<pickargs<3, T0, T1, T2, T3>::type, std::tuple<T0, T1, T2>>::value);
  static_assert(std::is_same<pickargs<4, T0, T1, T2, T3>::type, std::tuple<T0, T1, T2, T3>>::value);

  /* pickargs_R */

  static_assert(std::is_same<pickargs_r<0>::type, std::tuple<>>::value);

  static_assert(std::is_same<pickargs_r<0, T0>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs_r<1, T0>::type, std::tuple<T0>>::value);

  static_assert(std::is_same<pickargs_r<0, T0, T1>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs_r<1, T0, T1>::type, std::tuple<T1>>::value);
  static_assert(std::is_same<pickargs_r<2, T0, T1>::type, std::tuple<T0, T1>>::value);

  static_assert(std::is_same<pickargs_r<0, T0, T1, T2>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs_r<1, T0, T1, T2>::type, std::tuple<T2>>::value);
  static_assert(std::is_same<pickargs_r<2, T0, T1, T2>::type, std::tuple<T1, T2>>::value);
  static_assert(std::is_same<pickargs_r<3, T0, T1, T2>::type, std::tuple<T0, T1, T2>>::value);

  static_assert(std::is_same<pickargs_r<0, T0, T1, T2, T3>::type, std::tuple<>>::value);
  static_assert(std::is_same<pickargs_r<1, T0, T1, T2, T3>::type, std::tuple<T3>>::value);
  static_assert(std::is_same<pickargs_r<2, T0, T1, T2, T3>::type, std::tuple<T2, T3>>::value);
  static_assert(std::is_same<pickargs_r<3, T0, T1, T2, T3>::type, std::tuple<T1, T2, T3>>::value);
  static_assert(std::is_same<pickargs_r<4, T0, T1, T2, T3>::type, std::tuple<T0, T1, T2, T3>>::value);
}
