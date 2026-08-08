#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>

const char *BitcoinExchange::FileException::what() const throw()
{
	return ("Could not open file.");
}

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_db = other._db;
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string &s)
{
	size_t start = s.find_first_not_of(" \t");
	if (start == std::string::npos)
		return ("");
	size_t end = s.find_last_not_of(" \t");
	return (s.substr(start, end - start + 1));
}

bool BitcoinExchange::isLeapYear(int year) const
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

int BitcoinExchange::daysInMonth(int month, int year) const
{
	static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2 && isLeapYear(year))
		return (29);
	return (days[month - 1]);
}

bool BitcoinExchange::isValidDate(const std::string &date) const
{
	if (date.size() != 10)
		return (false);
	if (date[4] != '-' || date[7] != '-')
		return (false);
	for (size_t i = 0; i < date.size(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (year < 0 || month < 1 || month > 12)
		return (false);
	if (day < 1 || day > daysInMonth(month, year))
		return (false);
	return (true);
}

bool BitcoinExchange::isValidValue(const std::string &value, float &out) const
{
	if (value.empty())
		return (false);

	char *end = NULL;
	const char *cstr = value.c_str();
	double result = std::strtod(cstr, &end);

	if (end == cstr || *end != '\0')
		return (false);

	out = static_cast<float>(result);
	return (true);
}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw FileException();

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		size_t commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;

		std::string date = trim(line.substr(0, commaPos));
		std::string rateStr = trim(line.substr(commaPos + 1));

		if (!isValidDate(date))
			continue;

		float rate;
		if (!isValidValue(rateStr, rate))
			continue;

		_db[date] = rate;
	}
	file.close();
}

void BitcoinExchange::processInput(const std::string &filename) const
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw FileException();

	if (_db.empty())
	{
		std::cout << "Error: database is empty." << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);

	size_t sepHeader = line.find('|');
	if (sepHeader == std::string::npos)
	{
		std::cout << "Error: invalid header" << std::endl;
		return;
	}

	std::string dateHeader = trim(line.substr(0, sepHeader));
	std::string valueHeader = trim(line.substr(sepHeader + 1));

	if (dateHeader != "date" || valueHeader != "value") {
		std::cout << "Error: invalid header" << std::endl;
		return;
	}

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		size_t sep = line.find('|');
		if (sep == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = trim(line.substr(0, sep));
		std::string valueStr = trim(line.substr(sep + 1));

		if (!isValidDate(date))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		float value;
		if (!isValidValue(valueStr, value))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		if (value < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000)
		{
			std::cout << "Error: too large number." << std::endl;
			continue;
		}

		std::map<std::string, float>::const_iterator it = _db.lower_bound(date);
		if (it == _db.end())
			--it;
		else if (it->first != date)
		{
			if (it == _db.begin())
			{
				std::cout << "Error: no earlier rate available for => " << date << std::endl;
				continue;
			}
			--it;
		}

		double result = static_cast<double>(value) * static_cast<double>(it->second);
		std::cout << date << " => " << valueStr << " = " << result << std::endl;
	}
	file.close();
}
