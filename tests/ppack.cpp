#include <iostream>
#include <asyncfn_util/ppack.h>

using namespace asyncfn_util;

template <typename ...T> struct test {
  void operator() (T ...t) {}
};

void test_ppack() {
  std::cout << "test_ppack()" << std::endl;

  using T0 = int;
  using T1 = float;
  using T2 = double;
  using T3 = void*;

  {
    using pp = ppack<T0>;
    static_assert(pp::length == 1);
    static_assert(std::is_same<pp::get<0>, T0>::value);
    static_assert(std::is_same<pp::forward<test>, test<T0>>::value);
    static_assert(std::is_same<pp::range_forward<0, 1, test>, test<T0>>::value);
    pp::range_forward<0, 1, test>()(T0());
  }
  {
    using pp = ppack<T0, T1>;
    static_assert(pp::length == 2);
    static_assert(std::is_same<pp::get<0>, T0>::value);
    static_assert(std::is_same<pp::get<1>, T1>::value);
    static_assert(std::is_same<pp::forward<test>, test<T0, T1>>::value);
    static_assert(std::is_same<pp::range_forward<0, 1, test>, test<T0>>::value);
    static_assert(std::is_same<pp::range_forward<0, 2, test>, test<T0, T1>>::value);
    static_assert(std::is_same<pp::range_forward<1, 1, test>, test<T1>>::value);
    pp::range_forward<0, 1, test>()(T0());
    pp::range_forward<0, 2, test>()(T0(), T1());
    pp::range_forward<1, 1, test>()(T1());
  }
  {
    using pp = ppack<T0, T1, T2>;
    static_assert(pp::length == 3);
    static_assert(std::is_same<pp::get<0>, T0>::value);
    static_assert(std::is_same<pp::get<1>, T1>::value);
    static_assert(std::is_same<pp::get<2>, T2>::value);
    static_assert(std::is_same<pp::forward<test>, test<T0, T1, T2>>::value);
    static_assert(std::is_same<pp::range_forward<0, 1, test>, test<T0>>::value);
    static_assert(std::is_same<pp::range_forward<0, 2, test>, test<T0, T1>>::value);
    static_assert(std::is_same<pp::range_forward<0, 3, test>, test<T0, T1, T2>>::value);
    static_assert(std::is_same<pp::range_forward<1, 1, test>, test<T1>>::value);
    static_assert(std::is_same<pp::range_forward<1, 2, test>, test<T1, T2>>::value);
    static_assert(std::is_same<pp::range_forward<2, 1, test>, test<T2>>::value);
    pp::range_forward<0, 1, test>()(T0());
    pp::range_forward<0, 2, test>()(T0(), T1());
    pp::range_forward<0, 3, test>()(T0(), T1(), T2());
    pp::range_forward<1, 1, test>()(T1());
    pp::range_forward<1, 2, test>()(T1(), T2());
    pp::range_forward<2, 1, test>()(T2());
  }
  {
    using pp = ppack<T0, T1, T2, T3>;
    static_assert(pp::length == 4);
    static_assert(std::is_same<pp::get<0>, T0>::value);
    static_assert(std::is_same<pp::get<1>, T1>::value);
    static_assert(std::is_same<pp::get<2>, T2>::value);
    static_assert(std::is_same<pp::get<3>, T3>::value);
    static_assert(std::is_same<pp::forward<test>, test<T0, T1, T2, T3>>::value);
    static_assert(std::is_same<pp::range_forward<0, 1, test>, test<T0>>::value);
    static_assert(std::is_same<pp::range_forward<0, 2, test>, test<T0, T1>>::value);
    static_assert(std::is_same<pp::range_forward<0, 3, test>, test<T0, T1, T2>>::value);
    static_assert(std::is_same<pp::range_forward<0, 4, test>, test<T0, T1, T2, T3>>::value);
    static_assert(std::is_same<pp::range_forward<1, 1, test>, test<T1>>::value);
    static_assert(std::is_same<pp::range_forward<1, 2, test>, test<T1, T2>>::value);
    static_assert(std::is_same<pp::range_forward<1, 3, test>, test<T1, T2, T3>>::value);
    static_assert(std::is_same<pp::range_forward<2, 1, test>, test<T2>>::value);
    static_assert(std::is_same<pp::range_forward<2, 2, test>, test<T2, T3>>::value);
    static_assert(std::is_same<pp::range_forward<3, 1, test>, test<T3>>::value);
    pp::range_forward<0, 1, test>()(T0());
    pp::range_forward<0, 2, test>()(T0(), T1());
    pp::range_forward<0, 3, test>()(T0(), T1(), T2());
    pp::range_forward<0, 4, test>()(T0(), T1(), T2(), T3());
    pp::range_forward<1, 1, test>()(T1());
    pp::range_forward<1, 2, test>()(T1(), T2());
    pp::range_forward<1, 3, test>()(T1(), T2(), T3());
    pp::range_forward<2, 1, test>()(T2());
    pp::range_forward<2, 2, test>()(T2(), T3());
    pp::range_forward<3, 1, test>()(T3());
  }
}