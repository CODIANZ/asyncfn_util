#include <iostream>
#include <sstream>
#include <thread>
#include <asyncfn_util/lambda_enabler.h>
#include "sample_async_function.h"

using namespace asyncfn_util;

template <rcorder RC_ORDER, refconpos REFCON_POS, typename F> void test1(F func)
{
  std::cout << "begin test1<"
            << (RC_ORDER == rcorder::rc ? "rc" : "cr") << ","
            << (REFCON_POS == refconpos::first ? "first" : "last")
            << ">()" << std::endl;

  auto asyncfn = lambda_enabler<RC_ORDER, REFCON_POS, decltype(func)>(func);

  for(auto i = 0; i < 10; i++){
    asyncfn.prepare(i + 1, i + 3).call([=](auto ...n){
      const auto nn = std::make_tuple(n...);
      const auto nnn = REFCON_POS == refconpos::first ? 1 : 0;
      std::stringstream ss;
      ss << (std::get<nnn>(nn) * i) << std::endl;
      std::cout << ss.str();
    });
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::cout << "end test1()" << std::endl;
}

template <rcorder RC_ORDER, refconpos REFCON_POS, typename F> void test2(F func)
{
  std::cout << "begin test2<"
            << (RC_ORDER == rcorder::rc ? "rc" : "cr") << ","
            << (REFCON_POS == refconpos::first ? "first" : "last")
            << ">()" << std::endl;

#if __cplusplus >= 201703L  
  auto asyncfn = lambda_enabler<RC_ORDER, REFCON_POS, decltype(func)>(func);

  for(auto i = 0; i < 10; i++){
    asyncfn({i + 1, i + 3}, [=](auto ...n){
      const auto nn = std::make_tuple(n...);
      const auto nnn = REFCON_POS == refconpos::first ? 1 : 0;
      std::stringstream ss;
      ss << (std::get<nnn>(nn) * i) << std::endl;
      std::cout << ss.str();
    });
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));
#else
  std::cout << "less than C++17 -> thru" << std::endl;
#endif

  std::cout << "end test2()" << std::endl;
}

void test_lambda_enabler()
{
  std::cout << "test_lambda_enabler()" << std::endl;

  test1<rcorder::rc, refconpos::first >(asyncfunc_rc_first);
  test1<rcorder::rc, refconpos::last  >(asyncfunc_rc_last);
  test1<rcorder::cr, refconpos::first >(asyncfunc_cr_first);
  test1<rcorder::cr, refconpos::last  >(asyncfunc_cr_last);

  test2<rcorder::rc, refconpos::first >(asyncfunc_rc_first);
  test2<rcorder::rc, refconpos::last  >(asyncfunc_rc_last);
  test2<rcorder::cr, refconpos::first >(asyncfunc_cr_first);
  test2<rcorder::cr, refconpos::last  >(asyncfunc_cr_last);
}