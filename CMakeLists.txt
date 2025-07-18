cmake_minimum_required(VERSION 3.10)
project(server-proj)

file(GLOB SRCS "src/*.cpp" "src/cpp/*.cpp")
add_executable(server ${SRCS})
target_include_directories(server PRIVATE include src/include)
target_link_libraries(server PRIVATE pthread)