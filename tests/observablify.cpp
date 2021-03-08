#include <iostream>
#include <sstream>
#include <asyncfn_util/observablify.h>
#include "sample_async_function.h"

using namespace asyncfn_util;


template <rcorder RC_ORDER, refconpos REFCON_POS, typename F> void test1(F func)
{
  std::cout << "begin test3<"
            << (RC_ORDER == rcorder::rc ? "rc" : "cr") << ","
            << (REFCON_POS == refconpos::first ? "first" : "last")
            << ">()" << std::endl;

  auto rxfn = observablify<RC_ORDER, REFCON_POS, decltype(func)>(func);
  rxcpp::observable<>::range(0, 9)
  .flat_map([=](auto n){
    return rxfn.rx(n + 1, n + 3)
    .map([=](auto r){
      const auto nnn = REFCON_POS == refconpos::first ? 1 : 0;
      return std::get<nnn>(r) * n;
    }).as_dynamic();
  }).as_dynamic()
  .subscribe([](auto x){
    std::stringstream ss;
    ss << x << std::endl;
    std::cout << ss.str();
  });

  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::cout << "end test3()" << std::endl;
}

template <rcorder RC_ORDER, refconpos REFCON_POS, typename F> void test2(F func)
{
  std::cout << "begin test4<"
            << (RC_ORDER == rcorder::rc ? "rc" : "cr") << ","
            << (REFCON_POS == refconpos::first ? "first" : "last")
            << ">()" << std::endl;

#if __cplusplus >= 201703L  
  auto rxfn = observablify<RC_ORDER, REFCON_POS, decltype(func)>(func);
  rxcpp::observable<>::range(0, 9)
  .flat_map([=](auto n){
    return rxfn.rx({n + 1, n + 3})
    .map([=](auto r){
      const auto nnn = REFCON_POS == refconpos::first ? 1 : 0;
      return std::get<nnn>(r) * n;
    }).as_dynamic();
  }).as_dynamic()
  .subscribe([](auto x){
    std::stringstream ss;
    ss << x << std::endl;
    std::cout << ss.str();
  });

  std::this_thread::sleep_for(std::chrono::seconds(5));
#else
  std::cout << "less than C++17 -> thru" << std::endl;
#endif

  std::cout << "end test4()" << std::endl;
}


void test_observablify()
{
  std::cout << "test_observablify()" << std::endl;

  test1<rcorder::rc, refconpos::first >(asyncfunc_rc_first);
  test1<rcorder::rc, refconpos::last  >(asyncfunc_rc_last);
  test1<rcorder::cr, refconpos::first >(asyncfunc_cr_first);
  test1<rcorder::cr, refconpos::last  >(asyncfunc_cr_last);

  test2<rcorder::rc, refconpos::first >(asyncfunc_rc_first);
  test2<rcorder::rc, refconpos::last  >(asyncfunc_rc_last);
  test2<rcorder::cr, refconpos::first >(asyncfunc_cr_first);
  test2<rcorder::cr, refconpos::last  >(asyncfunc_cr_last);
}