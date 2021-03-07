# 非同期関数ユーティリティ

```関数ポインタ```を受け取る非同期関数をC++で扱いやすくするための機能を提供する。

## lambda_enabler

### 機能

```関数ポインタ```を引数に取る非同期関数でラムダ導入子付きのラムダ式を使えるようにする。

### 定義

```cpp
lambda_enabler<rcorder RC_ORDER, refconpos REFCON_POS, typename FUNCTION>(FUNCTION f);
```

#### recorder RC_ORDER

非同期関数で ```refcon``` と ```関数ポインタ``` の位置関係を指定します。

* ```rcorder::rc``` ... ```refcon```, ```関数ポインタ``` の順
* ```rcorder::rc``` ... ```関数ポインタ```, ```refcon``` の順

#### refconpos REFCON_POS

コールバック関数の引数で ```refcon``` の位置を指定します。

* ```refconpos::first``` ... 最初
* ```refconpos::last``` ... 最後

#### FUNCTION

関数型を指定します。通常 ```decltype(f)``` になります。


### 宣言例


なお、コールバック関数の ```void*``` は ```refcon``` を指すものとします。

#### パターン１

* コールバック関数が最終引数
* ```refcon``` が```関数ポインタ```の手前の引数
* コールバック関数の引数で```refcon```が最初の引数

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int, int));
auto asyncfn = lambda_enabler<rcorder::rc, refconpos::first, decltype(asyncfunc)>(asyncfunc);
```

#### パターン２

* コールバック関数が最終引数
* ```refcon``` が```関数ポインタ```の手前の引数
* コールバック関数の引数で```refcon```が最後の引数

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(int, int, void*));
auto asyncfn = lambda_enabler<rcorder::rc, refconpos::last, decltype(asyncfunc)>(asyncfunc);
```

#### パターン３

* ```refcon```が最終引数
* ```関数ポインタ```が```refcon```の手前の引数
* コールバック関数の引数で```refcon```が最初の引数

```cpp
extern "C" void asyncfunc(int p1, int p2, void(*callback)(void*, int, int), void* refcon);
auto asyncfn = lambda_enabler<rcorder::cr, refconpos::first, decltype(asyncfunc)>(asyncfunc);
```

#### パターン４

* ```refcon```が最終引数
* ```関数ポインタ```が```refcon```の手前の引数
* コールバック関数の引数で```refcon```が最後の引数

```cpp
extern "C" void asyncfunc(int p1, int p2, void(*callback)(int, int, void*), void* refcon);
auto asyncfn = lambda_enabler<rcorder::cr, refconpos::last, decltype(asyncfunc)>(asyncfunc);
```

### 使用方法

#### prepare() + call()

下記のような手順となる。

1. ```prepare()``` 関数に```refcon```と```関数ポインタ```を除いたパラメータを指定する。
2. ```call()``` により非同期関数が実行される。
3. 非同期関数が終了すると ```call()``` に渡した関数が呼び出される。

##### 使用例

```cpp
#include <asyncfn_util/lambda_enabler.h>

using namespace asyncfn_util;

extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int, int));

int main(void){
  auto asyncfn = lambda_enabler<rcorder::rc, refconpos::first, decltype(asyncfunc)>(asyncfunc);
  asyncfn.prepare(1, 2).call([=](void*, int r1, int r2){

  });
}
```

#### operator () 

C++17 以降であれば ```operator()``` を使用して次のように使用できる。
なお、```operator()``` の第１引数は、```refcon```と```関数ポインタ```を除いた値を ```std::tuple``` として渡す。

##### 使用例

```cpp
#include <asyncfn_util/lambda_enabler.h>

using namespace asyncfn_util;

extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int, int));

int main(void){
  auto asyncfn = lambda_enabler<rcorder::rc, refconpos::first, decltype(asyncfunc)>(asyncfunc);
  asyncfn({1, 2}, [=](void*, int r1, int r2){

  });
}
```

###　開発に至った背景や経緯

下記のような、```関数ポインタ```を引数に取る非同期関数があるとする。

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int));
```

```refcon``` はコールバックを呼び出す際にそのまま透過して受け取れるポインタ。

下記のラムダ式は問題なくコンパイルも動作もする。

```cpp
asyncfunc(1, 2, nullptr, [](void*, int resp){
  const int n = 10;
  std::cout << resp * n << std::endl;
});
```

しかし、ラムダ導入子を追加するとコンパイルエラーとなる。

```cpp
const int n = 10;
asyncfunc(1, 2, nullptr, [=](void*, int resp){
   std::cout << resp * n << std::endl;
});
```

ここで、ラムダ導入子を有するラムダ式を使えるようにしたいというのが開発の動機。

### 動作条件

* 非同期関数は中断せず、エラーでも成功でも必ずコールバック関数を呼び出すものであること。
* 非同期関数は ```refcon``` を受け取りコールバック関数にパスすること。
* 非同期関数の最後の２個の引数が、```関数ポインタ```と ```refcon``` であること。
* コールバック関数の引数の最初か最後が ```refcon``` であること
* ラムダ式のキャプチャ並びはコピーを推奨。

## observablify

### 機能

```関数ポインタ```を引数に取る非同期関数を rx の ```observable``` に変換する。

### 定義

```cpp
observablify<rcorder RC_ORDER, refconpos REFCON_POS, typename FUNCTION>(FUNCTION f);
```

各種パラメータは ```lambda_enabler``` と同様。

### 動作条件

 lambda_enabler と同様

### 使用方法

1. ```rx()``` 関数に```refcon```と```関数ポインタ```を除いたパラメータを指定することで ```observable``` が生成される。
2. 生成された```observable```を```subscribe() ```することで非同期関数が実行される。
3. 非同期関数が終了すると、非同期関数に渡されたパラメータを ```std::tuple``` 変換し、これを値として発行する。
4. 値を発行したのち直ちにコンプリートする。（エラーは発行しない）

なお、C++17以降では ```rx()``` で渡すべきパラメータを ```std::tuple``` で渡すことができる。

#### 使用例

```cpp
#include <asyncfn_util/observablify.h>

using namespace asyncfn_util;

extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int, int));

int main(void){
  auto asyncfn = observablify<rcorder::rc, refconpos::first, decltype(asyncfunc)>(asyncfunc);
  asyncfn.rx(1, 2)
  .subscribe([=](/* std::tuple<void*, int r1, int r2> */ auto r){
    const auto r1 = std::get<1>(r);
    const auto r2 = std::get<2>(r);
  });
}
```
