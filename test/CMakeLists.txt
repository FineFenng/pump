cmake_minimum_required(VERSION 3.8)
project(test)

include_directories("../include")


add_executable(LogTest LogTest.cpp)
target_link_libraries(LogTest pump_utiliy)

add_executable(SelectTest SelectTest.cpp)
target_link_libraries(SelectTest pump_net)







