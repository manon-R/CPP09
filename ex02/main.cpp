#include "PmergeMe.hpp"

int main(int argc, char **argv){

	if (argc < 2)
	{
		std::cout << "A sequence of positive integers is required as argument" << std::endl;
		return 0;
	}
	PmergeMe test;
	test.fjSort(argv);
	return 0;
}