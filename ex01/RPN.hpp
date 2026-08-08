#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <exception>

class RPN
{
	public:
		RPN();
		RPN(const RPN &other);
		RPN &operator=(const RPN &other);
		~RPN();

		static long evaluate(const std::string &expression);

		class SyntaxErrorException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class DivisionByZeroException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

#endif
