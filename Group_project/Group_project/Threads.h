#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <functional>
#include <map>
#include <sstream>
#include "linear_alg.h"
#include <ctime>

class algorithm
{
public:
	virtual vector<Matrix<double>> do_your_job(Matrix<double> data) = 0;
	virtual string shout() = 0;
};

#include "IteratorCSV.h"
using namespace std;

mutex cout_mutex;

template <size_t N, class T>
class FixedThreadPool
{
	mutex qm, qf;
	condition_variable done;
	deque<T> que;
	bool Finished[N];
	void reset()
	{
		for (auto& i : Finished)
		{
			i = false;
		}
	}
	void thread_task(size_t id)
	{
		while (true)
		{
			unique_lock<mutex> lock(qm);

			if (que.empty())
			{
				unique_lock<mutex> lock(qf);
				Finished[id] = true;
				done.notify_all();
				return;
			}
			auto job = move(que.front());
			que.pop_front();
			lock.unlock();
			job();
		}
	}
public:
	void push(T&& job)
	{
		unique_lock<mutex> lock(qm);
		que.emplace_back(move(job));
	}
	void start()
	{
		reset();
		for (size_t i = 0; i < N; i++)
		{
			thread worker([this, i]() {thread_task(i); });
			worker.detach();
		}
	}
	void wait_finished()
	{
		unique_lock<mutex> lock(qf);
		done.wait(lock, [this]
		{
			return all_of(Finished, Finished + N, [](bool e) { return e; });
		});
	}
};

void worker(algorithm* a, Matrix<double> data)
{
	vector<Matrix<double>> result = a->do_your_job(data);
	unsigned int t = clock();
	string new_name = to_string(t) + ".csv";
	write_in_file(new_name, result);
	{
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << new_name << "[" << std::this_thread::get_id() << "]" << std::endl;
	}
}

template <int N>
void paralel_alg(map<string, algorithm*> task_list)
{
	FixedThreadPool<N, function<void()>>pool;
	map<string, Matrix<double>> name_data = ClientCode(task_list);
	for (auto& j : task_list)
	{
		pool.push(bind(worker, j.second, name_data[j.first]));
	}
	pool.start();
	pool.wait_finished();
}