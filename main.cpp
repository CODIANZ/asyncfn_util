#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#include <asyncfn_util/lambda_enabler.h>
#include <asyncfn_util/observablify.h>

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

template <rcorder RC_ORDER, refconpos REFCON_POS, typename F> void test3(F func)
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
    std::cout << x << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::cout << "end test3()" << std::endl;
}

template <rcorder RC_ORDER, refconpos REFCON_POS, typename F> void test4(F func)
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
    std::cout << x << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(5));
#else
  std::cout << "less than C++17 -> thru" << std::endl;
#endif

  std::cout << "end test4()" << std::endl;
}

extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int)) {
  /* leaks the memory! but it's a test code :P */
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    callback(refcon, p1 + p2);
  });
}

extern "C" void asyncfunc2(int p1, int p2, void(*callback)(int, void*), void* refcon) {
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    callback(p1 + p2, refcon);
  });
}

int main(int, char**) {
  test1<rcorder::rc, refconpos::first>(asyncfunc);
  test2<rcorder::rc, refconpos::first>(asyncfunc);
  test3<rcorder::rc, refconpos::first>(asyncfunc);
  test4<rcorder::rc, refconpos::first>(asyncfunc);

  test1<rcorder::cr, refconpos::last>(asyncfunc2);
  test2<rcorder::cr, refconpos::last>(asyncfunc2);
  test3<rcorder::cr, refconpos::last>(asyncfunc2);
  test4<rcorder::cr, refconpos::last>(asyncfunc2);
}
