#include <thread>
#include <vector>
#include <iostream>
#include <pump/Common.h>

using namespace std;

typedef std::function<void()> Task;


void do_something(int a)
{
	std::cout << a << '\n';
}











Task task = callback;
task();



class event_loop {

public:
	void register_fd(int fd);
	void remove_fd(int fd);

	void poll();

private:
	std::mutex mutex_;
	TaskList task_list;
};



vector<std::vector<MySocket>> fd_list;


void WorkThread(int list_num)
{
	read_list, write_list;
	std::mutex  mutex_; //  main, sub
	TaskList task_list; //  main, sub

	Tasklist tmp_task_list;

	{
		std::lock_guard<std::mutex> lk(mutex_);
		std::swap(tmp_task_list, task_list);
	}


	// 1 execute task in tasklist;
	for (auto& i tmp_task_list)
	{
		i();
	}

	// 2 init fd_set	
	FD_SET(fd, ....);

	//3
	int re = select(0, &read_set, nullptr, nullptr, nullptr);

	//4 execute task in io_task_list;
	if (re > 0) {
			//TODO
	}
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
void bind()

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


int main()
{
	std::vector<Task> tasks;




	
	auto other_task = std::bind(do_something, 3);

	std::function<void()> a = other_task;


	Task new_task = std::bind(do_something, 3);

	




	
}
