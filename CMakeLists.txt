cmake_minimum_required(VERSION 3.10)
project(QPSTest)

set(CMAKE_CXX_STANDARD 11)

add_executable(Server EventLoop.cpp Server.cc)
add_executable(Client Client.cc)
