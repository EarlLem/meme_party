#include "Threads.h"
#include <string>
#include <iostream>

mutex cout_mutex;

void worker(char c) {
	{
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << c << "[" << std::this_thread::get_id() << "]" << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int func()
{
	FixedThreadPool<4, function<void()>>pool;
	string jobs{ "one string to rule them all" };
	for (auto j : jobs)
	{
		pool.push(bind(worker, j));
	}
	pool.start();
	pool.wait_finished();
	system("pause");
	return 0;
}