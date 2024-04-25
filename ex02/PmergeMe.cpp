#include "PmergeMe.hpp"


static std::vector<std::string> ft_split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

static bool isDigit(std::string& str){
	std::vector<std::string> test = ft_split(str, ' ');
	std::string::const_iterator its;
	for (std::vector<std::string>::iterator it = test.begin(); it != test.end(); ++it){
		char **end = NULL;
		const char *itc = it->c_str();
		long l = std::strtol(itc, end, 10);
		if (l > std::numeric_limits<int>::max())
			return false;
		for (its = it->begin(); its != it->end(); ++its)
			if (*its < '0' || *its > '9')
				return false;
	}
	return true;
}

static bool isValidArg(char **argv){
	int i = 1;
	while (argv[i])
	{
		std::string tmp = argv[i];
		if (tmp.empty() || !isDigit(tmp))
		{
			std::cout << "Error: Empty sequence or Invalid element (A sequence of positive integers is required)" << std::endl;
			return false;
		}
		i++;
	}
	return true;
}


static void binaryInsert(std::vector<Number> *final, const Number& n){
	int left = 0;
	int right = final->size() - 1;
	if (final->size() == 1)
	{
		if ((*final)[0].value > n.value)
			final->insert(final->begin(), n);
		else
			final->push_back(n);
	}
	else
	{
		while (left <= right) {
			int mid = left + (right - left) / 2;
			if ((*final)[mid].value < n.value) {
				left = mid + 1;
			} else {
				right = mid - 1;
			}
		}
		final->insert(final->begin() + left, n);
	}
}

PmergeMe::PmergeMe(): jacobsthalIndex(1){}

PmergeMe::~PmergeMe(){}

PmergeMe::PmergeMe(const PmergeMe& other){
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other){
	vSequence.clear();
	lSequence.clear();
	for (std::vector<Number>::const_iterator itV = other.vSequence.begin(); itV != other.vSequence.end(); ++itV){
		vSequence.push_back(*itV);
	}
	for (std::list<Number>::const_iterator itL = other.lSequence.begin(); itL != other.lSequence.end(); ++itL){
		vSequence.push_back(*itL);
	}
	return *this;
}

void PmergeMe::fjSort(char **argv){
	if (!isValidArg(argv))
		return ;
	if (!vPart(argv))
	{
		std::cout << "Error: This sequence is already sorted" << std::endl;
		return ;
	}
	// if (!lPart(argv))
	// 	return ;

	//Chaque algo doit recuperer les values dans son container respectif et avant de commencer verifier si la taille de la sequence est > 1 et 
	// si la sequence n'est pas deja triee 
}

bool PmergeMe::vPart(char **argv){
	//Demarrer le chrono ici
	std::vector<std::string> test;
	std::vector<Number> final;
	Number n;
	int i = 1;

	while (argv[i])
	{
		std::string tmp = argv[i];
		test = ft_split(tmp, ' ');
		for(unsigned long j = 0; j < test.size(); j++){
			n.value = atoi(test[j].c_str());
			vSequence.push_back(n);
		}
		i++;
	}
	if (isSorted(vSequence))
		return false;
	completePairsV();

	vCopy = vSequence;
	std::cout << "Before: ";
	displayContent(vCopy);

	sortPairsV();
	// if (isSorted(vSequence)) // Check si la sequence est deja triee
	// 	return finalDisplay();

	//Push les max dans un vector en commencant par le dernier element du vecteur
	n = vSequence[vSequence.size()-1];
	final.push_back(n);
	vSequence.pop_back();
	for(unsigned long i = 1; i < vSequence.size(); i+=2){
		if (i % 2 != 0){
			binaryInsert(&final, vSequence[i]);
		}
	}

	// Supprimer les elements qui ont ete push dans final 
	for(std::vector<Number>::iterator it = vSequence.begin(); it < vSequence.end(); ++it){
		if (it->value > it->pair)
			vSequence.erase(it);
	}
	// On commence par insérer le Number qui est lié au 1er Number de final
	for(std::vector<Number>::iterator it = vSequence.begin(); it < vSequence.end(); ++it){
		if (it->value == final.front().pair)
		{
			final.insert(final.begin(), *it);
			vSequence.erase(it);
			break;
		}
	}
	std::cout << "After: ";
	displayContent(vSequence);
	displayContent(final);
	std::cout << "Next Jacobsthal: " << nextJacobsthal(0, 2, 0) << std::endl;
	jacobsthalIndex++;
	std::cout << "Next Jacobsthal: " << nextJacobsthal(0, 2, 0) << std::endl;
	jacobsthalIndex++;
	std::cout << "Next Jacobsthal: " << nextJacobsthal(0, 2, 0) << std::endl;
	jacobsthalIndex++;
	std::cout << "Next Jacobsthal: " << nextJacobsthal(0, 2, 0) << std::endl;
	jacobsthalIndex++;
	std::cout << "Next Jacobsthal: " << nextJacobsthal(0, 2, 0) << std::endl;

	return true;
}


bool PmergeMe::isSorted(std::vector<Number>& vec){
	for(unsigned long i = 0; i < vec.size() - 1; i++){
		if (vec[i].value > vec[i + 1].value)
			return false;
	}
	return true;
}

template<typename T>
void PmergeMe::displayContent(T& t){
	for(typename T::iterator it = t.begin(); it != t.end(); ++it){
		std::cout << it->value << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::completePairsV(){
	for (unsigned long i = 0; i < vSequence.size(); i++)
	{
		if (i % 2 == 0 && (i + 1) < vSequence.size())
			vSequence[i].pair = vSequence[i+1].value;
		else if ((i + 1) < vSequence.size() || vSequence.size() % 2 == 0)
			vSequence[i].pair = vSequence[i-1].value;
		else
			vSequence[i].pair = -1;
	}
}

void PmergeMe::sortPairsV(){
	int tmpV;
	int tmpP;
	for (unsigned long i = 0; i < vSequence.size() - 1; i+= 2)
	{
		tmpV = vSequence[i].value;
		tmpP = vSequence[i].pair;
		if (tmpV > vSequence[i+1].value)
		{
			vSequence[i].value = vSequence[i+1].value;
			vSequence[i].pair = vSequence[i+1].pair;
			vSequence[i+1].value = tmpV;
			vSequence[i+1].pair = tmpP;
		}
	}
}

int PmergeMe::nextJacobsthal(int x1, int x2, unsigned int index){

	if (index == jacobsthalIndex)
		return x2; 
	index++;
	int tmp = x2 + (2 * x1);
	x1 = x2;
	x2 = tmp;
	return nextJacobsthal(x1, x2, index);
}


// bool PmergeMe::lPart(char **argv){
// 	//Demarrer le chrono ici
// 	return true;
// }


















/*// Fonction pour générer les nombres de Jacobsthal jusqu'à un certain seuil
std::vector<int> generateJacobsthal(int limit) {
	std::vector<int> jacobsthal;
	jacobsthal.push_back(0);
	jacobsthal.push_back(2);

	while (jacobsthal.back() <= limit) {
		int next = jacobsthal.size() > 1 ? jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2] : 0;
		jacobsthal.push_back(next);
	}

	return jacobsthal;
}*/