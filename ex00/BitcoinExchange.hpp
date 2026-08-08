#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>
#include <exception>

class BitcoinExchange
{
	private:
		std::map<std::string, float> _db;

		bool		isLeapYear(int year) const;
		int			daysInMonth(int month, int year) const;
		bool		isValidDate(const std::string &date) const;
		bool		isValidValue(const std::string &value, float &out) const;

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void	loadDatabase(const std::string &filename);
		void	processInput(const std::string &filename) const;

		static std::string trim(const std::string &s);

		class FileException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

#endif
