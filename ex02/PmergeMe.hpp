#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <vector>
#include <list>

struct Number {
	int value;
	int pair; // Value du nombre avec qui il est paired
};


class PmergeMe{

public:

	PmergeMe();
	~PmergeMe();

	void fjSort(char **argv);

private:

	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);

	bool vPart(char **argv);
	bool lPart(char **argv);


	std::vector<Number>vSequence;
	std::vector<Number>vCopy;
	std::list<Number>lSequence;
	unsigned int jacobsthalIndex;

	bool isSorted(std::vector<Number>& vec);
	void completePairsV();
	void sortPairsV();
	int nextJacobsthal(int x1, int x2, unsigned int index);

	template<typename T>
	void displayContent(T& t);
};


#endif