#if !defined(__h_sample_async_function__)
#define __h_sample_async_function__
extern "C" void asyncfunc_rc_first(int p1, int p2, void* refcon, void(*callback)(void*, int));
extern "C" void asyncfunc_rc_last(int p1, int p2, void* refcon, void(*callback)(int, void*));
extern "C" void asyncfunc_cr_first(int p1, int p2, void(*callback)(void*, int), void* refcon);
extern "C" void asyncfunc_cr_last(int p1, int p2, void(*callback)(int, void*), void* refcon);
#endif /* !defined(__h_sample_async_function__) */