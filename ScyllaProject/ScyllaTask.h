#pragma once

#include <iostream>
#include <cassert>
#include <mutex>
#include <thread>
#include <vector>

class A
{
public:
	template<typename T>
	std::vector<T> f(const std::vector<T>& vec);
public:
	static constexpr int num_threads = 8;
private:
	std::vector<std::thread> processors_;

	std::condition_variable condition_;
	std::mutex mutex_;
	bool ready_{ false };
private:
	std::vector<std::pair<int, int>> split_vector(int vec_size);
};

template<typename T>
std::vector<T> A::f(const std::vector<T>& vec)
{
	std::vector<T> new_vec;

	auto multiply = [&vec, &new_vec, this](int start, int end)
	{
		for (int i = start; i <= end; ++i)
		{
			std::unique_lock<std::mutex> localLock(mutex_);
			condition_.wait(localLock, [this]() { return ready_; });
			new_vec.push_back(vec[i] * 5);
		}

	};

	auto indexes = split_vector(vec.size());
	for (const auto pair : indexes)
	{
		processors_.emplace_back(std::thread(multiply, pair.first, pair.second));
		{
			std::lock_guard<std::mutex> localLock(mutex_);
			ready_ = true;
		}

		processors_[processors_.size() - 1].join();
		condition_.notify_one();
	}

	return std::move(new_vec);
}

std::vector<std::pair<int, int>> A::split_vector(int vec_size)
{
	const int div = vec_size / num_threads;
	const int mod = vec_size % num_threads;
	int index = 0;

	assert(vec_size >= num_threads);

	std::vector<std::pair<int, int>> splited_vec_indexes;

	for (int i = 0; i < num_threads - 1; ++i)
	{
		splited_vec_indexes.emplace_back(index, index + div - 1);
		index += div;
	}
	splited_vec_indexes.emplace_back(index, index + mod);

	return splited_vec_indexes;
}