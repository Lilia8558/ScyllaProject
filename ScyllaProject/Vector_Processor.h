#pragma once

#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Vector_Processor
{
public:
	template<typename T>
	std::vector<T> process(const std::vector<T>& vec);
public:
	static constexpr int num_threads = 8;
private:
	std::vector<std::thread> processors_;

	const int multiplier = 5;
	std::condition_variable condition_;
	std::mutex mutex_;
	bool ready_{ false };
private:
	using pair_array = std::vector<std::pair<int, int>>;
	pair_array split_vector(const int vec_size);
};

template<typename T>
std::vector<T> Vector_Processor::process(const std::vector<T>& vec)
{
	std::vector<T> new_vec;

	auto multiply = [&vec, &new_vec, this](int start, int end)
	{
		for (int i = start; i <= end; ++i)
		{
			std::unique_lock<std::mutex> localLock(mutex_);
			condition_.wait(localLock, [this]() { return ready_; });
			assert(i >= 0 && i < vec.size() && "Error: Incorrect index.");
			new_vec.push_back(vec[i] * multiplier);
		}
	};

	auto indexes = split_vector(vec.size());
	for (const auto& pair : indexes)
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

Vector_Processor::pair_array Vector_Processor::split_vector(const int vec_size)
{
	const int div = vec_size / num_threads;
	const int mod = vec_size % num_threads;
	int index = 0;

	assert(vec_size >= num_threads && "Error: Incorrect vector size.");

	pair_array splited_vec_indexes;

	for (int i = 0; i < num_threads - 1; ++i)
	{
		splited_vec_indexes.emplace_back(index, index + div - 1);
		index += div;
	}
	splited_vec_indexes.emplace_back(index, index + mod);

	return std::move(splited_vec_indexes);
}