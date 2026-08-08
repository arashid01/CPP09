#include "RPN.hpp"
#include <stack>
#include <sstream>
#include <cctype>

const char *RPN::SyntaxErrorException::what() const throw()
{
	return ("Error");
}

const char *RPN::DivisionByZeroException::what() const throw()
{
	return ("Error: division by zero.");
}

RPN::RPN() {}

RPN::RPN(const RPN &other)
{
	(void)other;
}

RPN &RPN::operator=(const RPN &other)
{
	(void)other;
	return (*this);
}

RPN::~RPN() {}

static bool isOperator(const std::string &token)
{
	return (token == "+" || token == "-" || token == "*" || token == "/");
}

long RPN::evaluate(const std::string &expression)
{
	std::stack<long>	values;
	std::istringstream	iss(expression);
	std::string			token;

	while (iss >> token)
	{
		if (isOperator(token) && token.size() == 1)
		{
			if (values.size() < 2)
				throw SyntaxErrorException();

			long rhs = values.top();
			values.pop();
			long lhs = values.top();
			values.pop();
			long result = 0;

			if (token == "+")
				result = lhs + rhs;
			else if (token == "-")
				result = lhs - rhs;
			else if (token == "*")
				result = lhs * rhs;
			else
			{
				if (rhs == 0)
					throw DivisionByZeroException();
				result = lhs / rhs;
			}
			values.push(result);
		}
		else if (token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0])))
			values.push(static_cast<long>(token[0] - '0'));
		else
			throw SyntaxErrorException();
	}

	if (values.size() != 1)
		throw SyntaxErrorException();

	return (values.top());
}
