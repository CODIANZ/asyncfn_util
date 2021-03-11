cmake_minimum_required(VERSION 3.0.0)
project(asyncfn_util VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(asyncfn_util
  main.cpp
  tests/omitargs.cpp
  tests/pickargs.cpp
  tests/sample_async_function.cpp
  tests/lambda_enabler.cpp
  tests/observablify.cpp
  tests/ppack.cpp
)

set_target_properties(asyncfn_util PROPERTIES
  CXX_STANDARD 14
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


target_include_directories(asyncfn_util PRIVATE ./RxCpp/Rx/v2/src)
target_include_directories(asyncfn_util PRIVATE ./src)

find_package(Threads REQUIRED)
target_link_libraries(asyncfn_util Threads::Threads)
