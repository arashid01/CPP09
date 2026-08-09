#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <ctime>

template <typename T>
void printContainer(const std::string &message, const T &container, int argc)
{
	std::cout << message;
	for (int i = 0; i < argc; ++i)
	{
		std::cout << container[i];
		std::cout << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error\n";
		return (1);
	}

	try
	{
		PmergeMe sorter;

		std::vector<long> tmp = sorter.parseArgsVec(argc, argv);
		printContainer("Before: ", argv + 1, argc - 1);

		clock_t startVec = clock();
		std::vector<long> vecInput = sorter.parseArgsVec(argc, argv);
		std::vector<long> vecSorted = sorter.sortVector(vecInput);
		clock_t endVec = clock();
		double timeVec = static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1000000;
		
		clock_t startDeq = clock();
		std::deque<long> deqInput = sorter.parseArgsDeq(argc, argv);
		std::deque<long> deqSorted = sorter.sortDeque(deqInput);
		clock_t endDeq = clock();
		double timeDeq = static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1000000;
		
		printContainer("After: ", vecSorted, vecSorted.size());
		std::cout << "Time to process a range of " << vecInput.size() << " elements with std::vector : " << timeVec << " us" << std::endl;
		std::cout << "Time to process a range of " << deqInput.size() << " elements with std::deque : " << timeDeq << " us" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	
	return (0);
}