cmake_minimum_required(VERSION 3.0.0)
project(asyncfn_util VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(asyncfn_util main.cpp)

# set_target_properties(asyncfn_util PROPERTIES
#   CXX_STANDARD 17
# )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(asyncfn_util PRIVATE ./RxCpp/Rx/v2/src)
target_include_directories(asyncfn_util PRIVATE ./src)

find_package(Threads REQUIRED)
target_link_libraries(asyncfn_util Threads::Threads)
