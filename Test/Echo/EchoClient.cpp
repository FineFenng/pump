#include <endian.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#include <vector>
#include <iostream>
#include <string>


/*
 * ClientPacke Structure
 * struct Package {
 *    int32_t message_len;
 *    char    message[messages_len]
 * }
 */

#define MAX_BUFFER_SIZE 1024
#define HEADER_SIZE 4

int32_t pos = 0;
int32_t  receive = 0;

char send_buffer[MAX_BUFFER_SIZE] = { 0 };
char recv_buffer[MAX_BUFFER_SIZE] = { 0 };


typedef std::vector<char> Package;

Package  BuildPackage(const std::string& message)
{
    int32_t host_len =  message.size();
    int32_t be_len = htobe32(host_len);
    int32_t package_len = HEADER_SIZE + host_len;

    std::vector<char> client_package;
    client_package.resize(package_len);

    memcpy(client_package.data(), &be_len, HEADER_SIZE);
    memcpy(client_package.data() + HEADER_SIZE, message.c_str(), host_len);

    return client_package;
}

int ConnectServer(const char* ip, unsigned short port, 
        struct sockaddr_in* local_address, struct sockaddr_in* peer_address )
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) { return -1; }

    struct sockaddr_in _address;
    ::bzero(&_address, 0);

    _address.sin_family = AF_INET;
    _address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &_address.sin_addr);

    int conn_re = connect(fd, reinterpret_cast<struct sockaddr*>(&_address), 
            sizeof(_address));

    if (conn_re < 0) { return -1; } 
    if (local_address != nullptr) { memcpy(local_address, &_address, sizeof(_address)); } 
    if (peer_address == nullptr) { return fd; }

    socklen_t peer_len = sizeof(*peer_address);
    getpeername(fd, reinterpret_cast<struct sockaddr*>(peer_address), &peer_len);

    return fd;
}

bool ParseData(std::string* message)
{
    if (receive >= HEADER_SIZE)
    {
        int32_t be_len = 0; 
        memcpy(&be_len, recv_buffer, HEADER_SIZE);
        int32_t message_len = be32toh(be_len);

        if (receive >= HEADER_SIZE +  message_len)
        {
            int32_t package_len = HEADER_SIZE + message_len;
            const char* start = recv_buffer + HEADER_SIZE;
            const char* end   = recv_buffer + package_len;
            message->assign(start, end);

            int32_t  remain  = receive -  package_len;
            assert(remain >= 0);

            if (remain > 0)
            {
                memmove(recv_buffer, recv_buffer + package_len, remain);
                receive = remain;
            }
            return true;
        }
    }
    return false;
}


int main(int argc, char* argv[])
{
    std::string message = "hello world!";

    Package package = BuildPackage(message);
    
    const char* ip = argv[1];
    const unsigned short port = atoi(argv[2]);

    bool is_print = atoi(argv[3]) == 0 ? false : true;

    struct sockaddr_in client_address, server_address;

    int conn_fd = ConnectServer(ip, port, &client_address, &server_address);

    /* Client send a package firstly...*/

    //write(conn_fd, package.data(), package.size());

    /* Client start revice and send package in loop...*/


    for(; ;)
    {
        
        int count = read(conn_fd, recv_buffer + receive, MAX_BUFFER_SIZE - receive);

        if (count == 0) 
        {
            close(conn_fd); 
            break;
        }

        receive += count;

        std::string server_message;
        if (ParseData(&server_message))
        {
            package = BuildPackage(server_message);
            write(conn_fd, package.data(), package.size());
            if (is_print) { printf("%s\n", server_message.c_str()); }
        }
    }
    return 0;
}


