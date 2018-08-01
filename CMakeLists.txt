cmake_minimum_required(VERSION 3.10)
project(QPSTest)

set(CMAKE_CXX_STANDARD 11)

add_executable(Server
        Platform.h
        Acceptor.cpp
        EventLoop.cpp
        Common.cpp
        Server.cc
        SocketAddress.cpp
        Socket.cpp
        TcpConnection.cpp)

add_executable(Client Client.cc)

