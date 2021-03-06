#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#include "lambda_enabler.h"

using namespace asyncfn_util;

extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int)) {
  /* leaks the memory! but it's a test code :P */
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    callback(refcon, p1 + p2);
  });
}

int main(int, char**) {
  auto asyncfn = lambda_enabler<0, decltype(asyncfunc)>(asyncfunc);

  for(auto i = 0; i < 10; i++){
    asyncfn.prepare(i + 1, i + 3).call([=](auto, auto n){
      std::stringstream ss;
      ss << "prepare -> call -> " << n * i << std::endl;
      std::cout << ss.str();
    });
#if __cplusplus >= 201703L
    asyncfn({i + 1, i + 3}, [=](auto, auto n){
      std::stringstream ss;
      ss << "operator ()  -> " << n * i << std::endl;
      std::cout << ss.str();
    });
#endif
  }

 std::this_thread::sleep_for(std::chrono::seconds(5));
}
