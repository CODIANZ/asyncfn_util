# 非同期関数ユーティリティ

関数ポインタを受け取る非同期関数をC++で扱いやすくするための機能を提供する。

## lambda_enabler

### 機能

関数ポインタを引数に取る非同期関数でラムダ導入子付きのラムダ式を使えるようにする。

### 動機

下記のような、関数ポインタを引数に取る非同期関数があるとする。

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

### 動作条件

* 非同期関数は中断せず、エラーでも成功でも必ずコールバック関数を呼び出すものであること。
* 非同期関数は refcon を受け取りコールバック関数にパスすること。
* 非同期関数の最終引数はコールバック関数への関数ポインタであること。
* 非同期関数の最終引数の１個前は refcon であること。
* ラムダ式のキャプチャ並びはコピーを推奨。

### 使用方法

下記のような非同期関数 ```asyncfunc()``` を想定する。

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int));
```

* ```callback``` の第１引数は ```refcon```。
* ```callback``` の第２引数は非同期関数により得られる何らかの数値。

#### コンストラクタ

```cpp
auto asyncfn = lambda_enabler<0, decltype(asyncfunc)>(asyncfunc);
```

```0``` は、 ```asyncfunc()``` の引数で```refcon``` が```0```番目の引数という指定。

#### 非同期関数の実行

```cpp
int i = 100;
asyncfn.prepare(1, 2).call([=](/* void* */ auto, /* int */ auto n){
  std::cout << i + n;
});
```

```asyncfunc()```　の　```refcon``` と ```callback``` を除いたパラメータを ```prepare() ``` に設定し、```call()```で呼び出しを実行。

#### 非同期関数の実行（C++17以降）

```cpp
int i = 100;
asyncfn({1, 2}, [=]((/* void* */ auto, /* int */ auto n){
  std::cout << i + n;
});
```

* 第１引数 ```{1, 2}``` は```asyncfunc()```　の　```refcon``` と ```callback``` を除いたパラメータ。
* 第２引数はコールバック関数。

## observablify

### 機能

関数ポインタを引数に取る非同期関数を rx の observable に変換する。

### 動作条件

 lambda_enabler と同様

### 使用方法

下記のような非同期関数 ```asyncfunc()``` を想定する。

```cpp
extern "C" void asyncfunc(int p1, int p2, void* refcon, void(*callback)(void*, int));
```

* ```callback``` の第１引数は ```refcon```。
* ```callback``` の第２引数は非同期関数により得られる何らかの数値。

#### コンストラクタ

```cpp
auto asyncfn = observablify<0, decltype(asyncfunc)>(asyncfunc);
```

```0``` は、 ```asyncfunc()``` の引数で```refcon``` が```0```番目の引数という指定。

#### 非同期関数の実行

```cpp
int i = 100;
observablify.rx(1, 2)
.subscribe([=](/* std::tuple<void*, int> */auto r){
  std::cout << i + std::get<1>(r);
});
```

```rx() ``` に ```asyncfunc()```　の　```refcon``` と ```callback``` を除いたパラメータを渡すことで observable が生成されます。コールバック関数が呼び出されると、パラメータを ```std::tuple``` にしたものを発行しコンプリートします。（エラー発行しません）

#### 非同期関数の実行（C++17以降）

```cpp
int i = 100;
observablify.rx({1, 2}) /* std::tuple<int, int> */
.subscribe([=](/* std::tuple<void*, int> */auto r){
  std::cout << i + std::get<1>(r);
});
```

```rx() ``` に ```asyncfunc()```　の　```refcon``` と ```callback``` を除いたパラメータを```std::tuple```にして渡すことで observable が生成されます。コールバック関数が呼び出されると、パラメータを ```std::tuple``` にしたものを発行しコンプリートします。（エラー発行しません）
