#include <thread>
#include <vector>
#include <pump/Common.h>

using namespace std;
typedef int MySocket;

vector<std::vector<MySocket>> fd_list;

int


void WorkThread(int list_num)
{
	do {

		struct timeval tv = {0, 0};
		vector<MySocket>& my_fd_list = fd_list[list_num];

		fd_set read_set;

		for (size_t i = 0; i < my_fd_list.size(); ++i) {
			FD_SET(my_fd_list[i], &read_set);
		}
		int re = select(0, &read_set, nullptr, nullptr, &tv);

		if (re > 0) {
			//TODO
		}
	} while ()
}

int main()
{
	const int thread_num = 4;

	fd_list.resize(thread_num);

	std::vector<std::thread> thread_list;

	for (int i = 0; i < thread_num; ++i) {

		std::vector<MySocket>* fd_list_addr = nullptr;
		std::thread th(WorkThread(i));
		thread_list.push_back(std::move(th));
	}

	const int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr{0, 0, 0, 0};
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = 0;
	server_addr.sin_port = 9999;

	::bind(listenfd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
	::listen(listenfd, 256);

	int dis_index = 0;
	for (;;) {
		int conn_fd = ::accept(listenfd, nullptr, nullptr);
		dis_index %= thread_num;
		fd_list[dis_index].push_back(conn_fd);
		dis_index++;
	}

	for (size_t i = 0; i < thread_list.size(); ++i) {

		if (thread_list[i].joinable()) {
			thread_list[i].join();
		}
	}
}
