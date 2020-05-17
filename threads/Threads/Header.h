#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>
using namespace std;

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
			return all_of(Finished, Finished + N - 1, [](bool e) {return e; });
		});
	}
};