cmake_minimum_required(VERSION 3.8)
project(pump)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -o3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o3")


############common_header 
file(GLOB HEADERS "include/pump/*.h")
############common_header 

############config_header
file(GLOB CONFIG_HEADERS "include/pump/config/*.h")
############config_header


############utility_header
file(GLOB_RECURSE UTILITY_HEADERS "include/pump/utility/*.h")
############utility_header


############net_header
file(GLOB NET_HEADERS        "include/pump/net/*.h")
file(GLOB NET_HANDLER_HEADERS "include/pump/net/handler/*.h")
file(GLOB NET_PARSER_HEADERS "include/pump/net/parser/*.h")
file(GLOB NET_SERVER_HEADERS "include/pump/net/server/*.h")

set(NET_POLLER_HEADERS)
if(WIN32)
	file(GLOB NET_POLLER_HEADERS "include/pump/net/poller/Select.h")
else(WIN32)
	if(APPLE)
		file(GLOB NET_POLLER_HEADERS "include/pump/net/poller/Select.h" "include/pump/net/poller/Poll.h")
	else(APPLE)
		file(GLOB NET_POLLER_HEADERS "include/pump/net/poller/Select.h" "include/pump/net/poller/Poll.h" "include/pump/net/poller/Epoll.h")
	endif(APPLE)
endif(WIN32)
###########net_header

###########utility_src
set(UTILITY_SRCS 
	src/utility/log/Logger.cpp 	
	src/utility/log/LogStream.cpp
	)
###########utility_src


###########net_src
set(NET_SRCS
    src/net/Acceptor.cpp
    src/net/Buffer.cpp
    src/net/EventLoop.cpp
    src/net/SocketAddress.cpp
	src/net/SocketOption.cpp
    src/net/TcpConnection.cpp
    src/net/TcpServer.cpp
    src/net/watcher/IOWatcher.cpp
	src/net/EventLoopThread.cpp
    )


if(WIN32)
	list(APPEND NET_SRCS src/net/poller/Select.cpp)
else(WIN32)
	if (APPLE)
		list(APPEND NET_SRCS src/net/poller/Select.cpp src/net/poller/Poll.cpp src/net/poller/Kqueue.cpp)
	else(APPLE) #Linux
		list(APPEND NET_SRCS src/net/poller/Select.cpp src/net/poller/Poll.cpp src/net/poller/Epoll.cpp)
	endif(APPLE)
endif(WIN32)
###########net_src


add_library(pump_utiliy STATIC
						${UTILITY_SRCS} 
						${UTILITY_HEADERS}
						)

add_library(pump_net  STATIC
					 ${NET_SRCS} 
					 ${HEADERS} 
					 ${CONFIG_HEADERS} 
					 ${NET_HEADERS} 
					 ${NET_HANDLER_HEADERS}
					 ${NET_POLLER_HEADERS} 
					 ${NET_PARSER_HEADERS}
					 ${NET_SERVER_HEADERS}
					 )

set(3rd_DIR "${CMAKE_SOURCE_DIR}/3rd")

add_library(node_http_parser STATIC
				${3rd_DIR}/http-parser/http_parser.c 
				${3rd_DIR}/http-parser/http_parser.h
				)


target_link_libraries(pump_net pump_utiliy node_http_parser)

target_include_directories(pump_net
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(pump_utiliy
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


add_executable(EchoTest test/EchoTest.cpp)
target_link_libraries(EchoTest pump_net)

add_executable(LogTest test/LogTest.cpp)
target_link_libraries(LogTest pump_utiliy)

add_executable(ChronoTest test/ChronoTest.cpp)









