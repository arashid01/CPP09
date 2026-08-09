#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <iostream>

const char *PmergeMe::ParseException::what() const throw()
{
	return ("Error");
}

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	(void)other;
	return (*this);
}

PmergeMe::~PmergeMe() {}


long PmergeMe::parseToken(const std::string &token)
{
	if (token.empty())
		throw ParseException();

	size_t idx = 0;
	if (token[0] == '+')
		idx = 1;
	if (idx >= token.size())
		throw ParseException();

	for (size_t j = idx; j < token.size(); j++)
	{
		if (!std::isdigit(static_cast<unsigned char>(token[j])))
			throw ParseException();
	}

	char *end = NULL;
	long value = std::strtol(token.c_str(), &end, 10);

	if (end == token.c_str() || *end != '\0')
		throw ParseException();
	if (value > INT_MAX || value <= 0)
		throw ParseException();

	return (value);
}

std::vector<long> PmergeMe::parseArgsVec(int argc, char **argv)
{
	std::vector<long> result;

	for (int i = 1; i < argc; i++)
	{
		std::string arg(argv[i]);
		size_t pos = 0;

		while (pos < arg.size())
		{
			while (pos < arg.size() && std::isspace(static_cast<unsigned char>(arg[pos])))
				pos++;
			if (pos >= arg.size())
				break;

			size_t start = pos;
			while (pos < arg.size() && !std::isspace(static_cast<unsigned char>(arg[pos])))
				pos++;

			std::string token = arg.substr(start, pos - start);
			result.push_back(parseToken(token));
		}
	}
	return (result);
}

std::deque<long> PmergeMe::parseArgsDeq(int argc, char **argv)
{
	std::deque<long> result;

	for (int i = 1; i < argc; i++)
	{
		std::string arg(argv[i]);
		size_t pos = 0;

		while (pos < arg.size())
		{
			while (pos < arg.size() && std::isspace(static_cast<unsigned char>(arg[pos])))
				pos++;
			if (pos >= arg.size())
				break;

			size_t start = pos;
			while (pos < arg.size() && !std::isspace(static_cast<unsigned char>(arg[pos])))
				pos++;

			std::string token = arg.substr(start, pos - start);
			result.push_back(parseToken(token));
		}
	}
	return (result);
}


std::vector<size_t> PmergeMe::jacobsthalOrder(size_t m)
{
	std::vector<size_t> order;

	if (m <= 1)
		return (order);

	std::vector<long> jac;
	jac.push_back(0);
	jac.push_back(1);
	while (static_cast<size_t>(jac.back()) < m)
		jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);

	size_t prevBound = 1;
	for (size_t k = 3; k < jac.size(); k++)
	{
		size_t curBound = static_cast<size_t>(jac[k]);
		size_t hi = std::min(curBound, m);

		if (prevBound < hi)
		{
			for (size_t v = hi; v > prevBound; v--)
				order.push_back(v - 1);
		}
		prevBound = curBound;
		if (prevBound >= m)
			break;
	}
	return (order);
}


std::vector<long> PmergeMe::sortVector(std::vector<long> input)
{
	size_t n = input.size();

	if (n <= 1)
		return (input);

	size_t m = n / 2;
	bool hasStraggler = (n % 2 == 1);
	long straggler = hasStraggler ? input[n - 1] : 0;

	std::vector<long> largers(m);
	std::vector<long> smallers(m);

	for (size_t i = 0; i < m; i++)
	{
		long a = input[2 * i];
		long b = input[2 * i + 1];
		if (a < b)
		{
			smallers[i] = a;
			largers[i] = b;
		}
		else
		{
			smallers[i] = b;
			largers[i] = a;
		}
	}

	std::vector<long> chain = sortVector(largers);

	std::vector<long>::iterator pos =
		std::lower_bound(chain.begin(), chain.end(), smallers[0]);
	chain.insert(pos, smallers[0]);

	std::vector<size_t> order = jacobsthalOrder(m);
	for (size_t k = 0; k < order.size(); k++)
	{
		size_t i = order[k];
		std::vector<long>::iterator bound =
			std::lower_bound(chain.begin(), chain.end(), largers[i]);
		std::vector<long>::iterator ipos =
			std::lower_bound(chain.begin(), bound, smallers[i]);
		chain.insert(ipos, smallers[i]);
	}

	if (hasStraggler)
	{
		std::vector<long>::iterator spos =
			std::lower_bound(chain.begin(), chain.end(), straggler);
		chain.insert(spos, straggler);
	}

	return (chain);
}


std::deque<long> PmergeMe::sortDeque(std::deque<long> input)
{
	size_t n = input.size();
	if (n <= 1)
		return (input);

	size_t m = n / 2;
	bool hasStraggler = (n % 2 == 1);
	long straggler = hasStraggler ? input[n - 1] : 0;

	std::deque<long> largers(m);
	std::deque<long> smallers(m);

	for (size_t i = 0; i < m; i++)
	{
		long a = input[2 * i];
		long b = input[2 * i + 1];
		if (a < b)
		{
			smallers[i] = a;
			largers[i] = b;
		}
		else
		{
			smallers[i] = b;
			largers[i] = a;
		}
	}

	std::deque<long> chain = sortDeque(largers);

	std::deque<long>::iterator pos =
		std::lower_bound(chain.begin(), chain.end(), smallers[0]);
	chain.insert(pos, smallers[0]);

	std::vector<size_t> order = jacobsthalOrder(m);
	for (size_t k = 0; k < order.size(); k++)
	{
		size_t i = order[k];
		std::deque<long>::iterator bound =
			std::lower_bound(chain.begin(), chain.end(), largers[i]);
		std::deque<long>::iterator ipos =
			std::lower_bound(chain.begin(), bound, smallers[i]);
		chain.insert(ipos, smallers[i]);
	}

	if (hasStraggler)
	{
		std::deque<long>::iterator spos =
			std::lower_bound(chain.begin(), chain.end(), straggler);
		chain.insert(spos, straggler);
	}

	return (chain);
}
