#include "RPN.hpp"

static bool isOperator(char c){
	return (c == '+' || c == '-' || c == '/' || c == '*');
}

static bool isValidChar(char c){
	return ((c >= '0' && c <= '9') || c == ' ' || isOperator(c) == true);
}


RPN::RPN(){}

RPN::RPN(std::string& line){
	rpn_algo(line);
}

RPN::~RPN(){
}

RPN::RPN(const RPN& other){
	while(!rpnStack.empty())
		rpnStack.pop();
	*this = other;
}

RPN& RPN::operator=(const RPN& other){
	while(!rpnStack.empty())
		rpnStack.pop();
	rpnStack = other.rpnStack;
	return *this;
}

bool RPN::compute(char c){
	if (rpnStack.size() < 2)
	{
		std::cout << "Error: Not enough elements to calculate (1 operation need 2 elements)" << std::endl;
			return false;
	}
	double second = rpnStack.top();
	rpnStack.pop();
	double first = rpnStack.top();
	rpnStack.pop();
	switch(c){
		case '+':
		{
			rpnStack.push(first + second);
			return true;
		}
		case '-':
		{
			rpnStack.push(first - second);
			return true;
		}
		case '*':
		{
			rpnStack.push(first * second);
			return true;
		}
		case '/':
		{
			if (second == 0)
			{
				std::cout << "Error: division by 0 is impossible" << std::endl;
				return false;
			}
			rpnStack.push(first / second);
			return true;
		}
		default:
			break;
	}
	return true;
}

void RPN::rpn_algo(std::string& line){
	std::string::iterator it;
	for (it = line.begin(); it != line.end(); ++it){
		if (!isValidChar(*it))
		{
			std::cout << "Error: Invalid element" << std::endl;
			return ;
		}
		if (!isOperator(*it) && *it != ' '){
			rpnStack.push(*it - '0');
		}
		else if (isOperator(*it))
		{
			if (!compute(*it))
				return;
		}
	}
	if (rpnStack.size() != 1){
		std::cout << "Error: More than 1 element are still in the stack" << std::endl;
		return ;
	}
	std::cout << rpnStack.top() << std::endl;

}