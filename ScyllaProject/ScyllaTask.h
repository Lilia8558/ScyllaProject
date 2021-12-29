#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>

class A
{
public:
	template<typename T>
	std::vector<T> f(const std::vector<T>& vec);
public:
	static constexpr int num_threads = 8;
private:
	std::vector<std::thread> processors_;
	std::mutex mutex_;

	std::function<void(int, int)> multiply;
};

template<typename T>
std::vector<T> A::f(const std::vector<T>& vec)
{

	std::vector<T> new_vec;
	//std::cout << "NUM is: " << num << std::endl;
	int index = 0;

	multiply = [&vec, &new_vec, this](int start, int end)
	{
		for (int i = start; i <= end; ++i)
		{
			std::lock_guard<std::mutex> myLock(mutex_);
			new_vec.push_back(vec[i] * 5);
		}

		std::cout << "From Thread ID : " << std::this_thread::get_id() << "\n";
	};

	const int num = vec.size() / num_threads;
	for (int i = 0; i < num_threads; ++i)
	{
		//std::cout << "start: " << index << "*********" << "end: " << index + num - 1 << std::endl;

		processors_.emplace_back(std::thread(multiply, index, index + num - 1));
		index += num;
	}

	for (std::thread& th : processors_)
	{
		if (th.joinable())
		{
			th.join();
		}
	}

	return std::move(new_vec);
}