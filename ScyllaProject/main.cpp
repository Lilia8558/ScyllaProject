#include "ScyllaTask.h"

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8};

	A a;

	std::vector<int> newVec = a.f(vec);

	std::cout << "*******************************"<<std::endl;
	for (const auto& element : newVec)
	{
		std::cout << element << " ";
	}
}