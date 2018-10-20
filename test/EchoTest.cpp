#include <pump/net/server/EchoServer.h>
#include <pump/utility/log/LogLine.h>

int main(int argc, char* argv[])
{
	pump::utility::LogLine::set_global_log_level(pump::utility::LogLevel::PUMP_TRACE);

	const pump::net::SocketAddress address(8888);
	pump::net::EventLoop loop;
	pump::net::EchoServer server(&loop, address);
	loop.run();
	return 0;
}
