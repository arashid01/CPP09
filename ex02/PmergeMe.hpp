#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <exception>

class PmergeMe
{
	private:
		static std::vector<size_t>	jacobsthalOrder(size_t m);

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		static std::vector<long>	parseArgsVec(int argc, char **argv);
		static std::deque<long>		parseArgsDeq(int argc, char **argv);
		static long					parseToken(const std::string &token);

		static std::vector<long>	sortVector(std::vector<long> input);
		static std::deque<long>		sortDeque(std::deque<long> input);

		class ParseException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

#endif