#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#include <asyncfn_util/lambda_enabler.h>
#include <asyncfn_util/observablify.h>

using namespace asyncfn_util;

extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int)) {
  /* leaks the memory! but it's a test code :P */
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    callback(refcon, p1 + p2);
  });
}

void test1()
{
  std::cout << "begin test1()" << std::endl;

  auto asyncfn = lambda_enabler<0, decltype(asyncfunc)>(asyncfunc);

  for(auto i = 0; i < 10; i++){
    asyncfn.prepare(i + 1, i + 3).call([=](auto, auto n){
      std::stringstream ss;
      ss << n * i << std::endl;
      std::cout << ss.str();
    });
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::cout << "end test1()" << std::endl;
}

void test2()
{
  std::cout << "begin test2()" << std::endl;

#if __cplusplus >= 201703L  
  auto asyncfn = lambda_enabler<0, decltype(asyncfunc)>(asyncfunc);

  for(auto i = 0; i < 10; i++){
    asyncfn({i + 1, i + 3}, [=](auto, auto n){
      std::stringstream ss;
      ss << n * i << std::endl;
      std::cout << ss.str();
    });
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));
#else
  std::cout << "less than C++17 -> thru" << std::endl;
#endif

  std::cout << "end test2()" << std::endl;
}

void test3()
{
  std::cout << "begin test3()" << std::endl;

  auto rxfn = observablify<0, decltype(asyncfunc)>(asyncfunc);
  rxcpp::observable<>::range(0, 9)
  .flat_map([=](auto n){
    return rxfn.rx(n + 1, n + 3)
    .map([=](auto r){
      return std::get<1>(r) * n;
    }).as_dynamic();
  }).as_dynamic()
  .subscribe([](auto x){
    std::cout << x << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::cout << "end test3()" << std::endl;
}

void test4()
{
  std::cout << "begin test4()" << std::endl;

#if __cplusplus >= 201703L  
  auto rxfn = observablify<0, decltype(asyncfunc)>(asyncfunc);
  rxcpp::observable<>::range(0, 9)
  .flat_map([=](auto n){
    return rxfn.rx({n + 1, n + 3})
    .map([=](auto r){
      return std::get<1>(r) * n;
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

int main(int, char**) {
  test1();
  test2();
  test3();
  test4();
}
