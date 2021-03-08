#include <thread>
#include <chrono>
#include <random>

static auto get_random() {
  static std::random_device rnd;
  static std::mt19937 mt(rnd());
  static std::uniform_int_distribution<> rdest(1000, 3000);
  return rdest(mt);
}

extern "C" void asyncfunc_rc_first(int p1, int p2, void* refcon, void(*callback)(void*, int)) {
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::microseconds(get_random()));
    callback(refcon, p1 + p2);
  });
}

extern "C" void asyncfunc_rc_last(int p1, int p2, void* refcon, void(*callback)(int, void*)) {
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::microseconds(get_random()));
    callback(p1 + p2, refcon);
  });
}

extern "C" void asyncfunc_cr_first(int p1, int p2, void(*callback)(void*, int), void* refcon) {
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::microseconds(get_random()));
    callback(refcon, p1 + p2);
  });
}

extern "C" void asyncfunc_cr_last(int p1, int p2, void(*callback)(int, void*), void* refcon) {
  new std::thread([=](){
    std::this_thread::sleep_for(std::chrono::microseconds(get_random()));
    callback(p1 + p2, refcon);
  });
}