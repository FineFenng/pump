//
// Created by fenng on 18-8-8.
//


#include "../../include/pump/Buffer.h"
#include "../../include/pump/EventLoop.h"
#include "../../include/pump/TcpServer.h"

#include <sys/socket.h>
#include <iostream>

#include "LogTool.h"

#define HEADER_SIZE 4

typedef std::vector<char> Package;

Package  BuildPackage(const std::string& message)
{
    int32_t host_len = static_cast<int32_t>(message.size());
    int32_t be_len = htobe32(host_len);
    int32_t package_len = HEADER_SIZE + host_len;

    std::vector<char> client_package;
    client_package.resize(package_len);

    memcpy(client_package.data(), &be_len, HEADER_SIZE);
    memcpy(client_package.data() + HEADER_SIZE, message.c_str(), host_len);

    return client_package;
}


void init_connection_callback(const TcpConnection_Ptr& connection_ptr)
{
    std::string message = "hello world!";
    Package package = BuildPackage(message);
    connection_ptr->send(package.data(), package.size());
}

void message_callback(const TcpConnection_Ptr& connection_ptr, Buffer* buffer)
{
    std::string client_message = buffer->retrieve_all_to_string();
    //printf("%s\n", "hello world!");

    auto package = BuildPackage("hello world!");

    connection_ptr->send(package.data(), package.size());
}



int main()
{
    utility::log_file();
    const unsigned short port = 8888;
    EventLoop loop;

    TcpServer server(&loop,  port);
    server.set_new_connection_callback(init_connection_callback);
    server.set_message_readable_callback(message_callback);
    loop.run();

    utility::close_log();
}



