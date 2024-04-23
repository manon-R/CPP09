#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <stack>

class RPN{

public:
	RPN(std::string& line);
	~RPN();

private:

	RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);

	std::stack<double> rpnStack;

	void rpn_algo(std::string& line);
	bool compute(char c);

};

#endif