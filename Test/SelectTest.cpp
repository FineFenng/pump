#include <pump/net/server/EchoServer.h>

int main(int argc ,char* argv[])
{
	
    const pump::net::SocketAddress address(8888);
	pump::net::EventLoop loop;
    pump::net::EchoServer server(&loop, address);
    loop.run();
    return 0;
}