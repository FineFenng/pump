cmake_minimum_required(VERSION 3.10)
project(QPSTest)

set(CMAKE_CXX_STANDARD 11)

add_executable(Server Acceptor.cpp EventLoop.cpp Common.cpp Server.cc SocketAddress.cpp SocketAddress.h Socket.cpp Socket.h Handle.h Platform.h TcpConnection.cpp TcpConnection.h)
add_executable(Client Client.cc)
add_executable(Mahjong mahjong2.cpp)

