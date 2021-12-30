#include "ScyllaTask.h"

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

	Vector_Processor processor;

	std::vector<int> newVec = processor.process(vec);

	for (const auto& element : newVec)
	{
		std::cout << element << " ";
	}
}