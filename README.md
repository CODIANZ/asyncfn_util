# 非同期関数ユーティリティ

```関数ポインタ```を受け取る非同期関数をC++で取り扱いやすくするための機能を提供します。

## 開発に至った動機

下記のような、```関数ポインタ```を引数に取る非同期関数があります。

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int));
```

なお、```refcon``` は非同期関数 ```asyncfunc()``` がコールバック関数```callback```を呼び出す際にそのまま透過して引数に透過します。そして、コールバック関数は非同期関数を呼び出した際の ```refcon``` を得られます。一般的にはこの ```refcon``` に何らかのオブジェクトインスタンスへのポインタを渡すことが多いと思います。

ところで、下記のラムダ式は問題なくコンパイルも動作します。

```cpp
asyncfunc(1, 2, nullptr, [](void*, int resp){
  const int n = 10;
  std::cout << resp * n << std::endl;
});
```

しかし、ラムダ導入子を追加するとコンパイルエラーとなります。

```cpp
const int n = 10;
asyncfunc(1, 2, nullptr, [=](void*, int resp){
   std::cout << resp * n << std::endl;
});
```

そこで、```関数ポインタ```を受けるタイプの非同期関数で、ラムダ導入子を有するラムダ式を使えるようにしたいというのが、この ```asyncfn_util``` の開発動機になります。


## lambda_enabler

### 機能

```関数ポインタ```を引数に取る非同期関数でラムダ導入子付きのラムダ式を使えるようにします。

### 定義

```cpp
lambda_enabler<rcorder RC_ORDER, refconpos REFCON_POS, typename FUNCTION>(FUNCTION f);
```

#### recorder RC_ORDER

非同期関数で ```refcon``` と ```関数ポインタ``` の位置関係を指定します。

* ```rcorder::rc``` ... ```refcon```, ```関数ポインタ``` の順。
* ```rcorder::cr``` ... ```関数ポインタ```, ```refcon``` の順。

#### refconpos REFCON_POS

コールバック関数の引数で ```refcon``` の位置を指定します。

* ```refconpos::first``` ... 最初。
* ```refconpos::last``` ... 最後。

#### FUNCTION

関数型を指定します。通常 ```decltype(f)``` になります。


### 宣言例

コールバック関数引数での ```void*``` は ```refcon``` を指すものとし、以下に４つのパターンを例示します。

#### パターン１

* コールバック関数が最終引数。
* ```refcon``` が```関数ポインタ```の手前の引数。
* コールバック関数の引数で```refcon```が最初の引数。

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int, int));
auto asyncfn = lambda_enabler<rcorder::rc, refconpos::first, decltype(asyncfunc)>(asyncfunc);
```

#### パターン２

* コールバック関数が最終引数。
* ```refcon``` が```関数ポインタ```の手前の引数。
* コールバック関数の引数で```refcon```が最後の引数。

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(int, int, void*));
auto asyncfn = lambda_enabler<rcorder::rc, refconpos::last, decltype(asyncfunc)>(asyncfunc);
```

#### パターン３

* ```refcon```が最終引数。
* ```関数ポインタ```が```refcon```の手前の引数。
* コールバック関数の引数で```refcon```が最初の引数。

```cpp
extern "C" void asyncfunc(int p1, int p2, void(*callback)(void*, int, int), void* refcon);
auto asyncfn = lambda_enabler<rcorder::cr, refconpos::first, decltype(asyncfunc)>(asyncfunc);
```

#### パターン４

* ```refcon```が最終引数。
* ```関数ポインタ```が```refcon```の手前の引数。
* コールバック関数の引数で```refcon```が最後の引数。

```cpp
extern "C" void asyncfunc(int p1, int p2, void(*callback)(int, int, void*), void* refcon);
auto asyncfn = lambda_enabler<rcorder::cr, refconpos::last, decltype(asyncfunc)>(asyncfunc);
```

### 使用方法

#### prepare() + call()

下記の手順にて使用します。

1. ```prepare()``` 関数に```refcon```と```関数ポインタ```を除いたパラメータを指定します。
2. ```call()``` により非同期関数が実行されます。
3. 非同期関数が終了すると ```call()``` に渡した関数が呼び出されます。

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

C++17 以降であれば ```operator()``` を使用することができます。
```operator()``` の第１引数は、```refcon```と```関数ポインタ```を除いた値を ```std::tuple``` に変換したもので、第２引数はコールバック関数になります。

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

### 動作条件

* 非同期関数は中断せず、エラーでも成功でも必ずコールバック関数を呼び出すものであること。
* 非同期関数は ```refcon``` を受け取りコールバック関数にパスすること。
* 非同期関数の最後の２個の引数が、```関数ポインタ```と ```refcon``` であること。
* コールバック関数の引数の最初か最後が ```refcon``` であること。
* ラムダ式のキャプチャ並びはコピーを推奨。

## observablify

### 機能

```関数ポインタ```を引数に取る非同期関数を ```rxcpp``` の ```observable``` に変換します。

### 定義

```cpp
observablify<rcorder RC_ORDER, refconpos REFCON_POS, typename FUNCTION>(FUNCTION f);
```

各種パラメータ、動作条等は ```lambda_enabler``` と同様です。

### 使用方法

1. ```rx()``` 関数に```refcon```と```関数ポインタ```を除いたパラメータを指定することで ```observable``` が生成されます。
2. 生成された```observable```を```subscribe() ```することで非同期関数が実行されます。
3. 非同期関数が終了すると、非同期関数に渡されたパラメータを ```std::tuple``` 変換し、これを値として発行します。
4. 値を発行したのち直ちにコンプリートします。（エラーは発行しません）

なお、C++17以降では ```rx()``` で渡すべきパラメータを ```std::tuple``` で渡すことができます。

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
