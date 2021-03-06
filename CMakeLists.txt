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
find_package(Threads REQUIRED)
include(CPack)
target_link_libraries(asyncfn_util Threads::Threads)