#include "RPN.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if (argc != 2 || !argv[1][0])
	{
		cout << "One argument required (Reverse Polish Notation expression)" << endl;
		return 0;
	}
	std::string expression = argv[1];
	RPN rpn(expression);
}