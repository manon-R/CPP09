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

static std::list<std::string> ft_split_list(const std::string& str, char delimiter) {
	std::list<std::string> tokens;
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

static int rightIndex(std::vector<Number> *final, int value)
{
	for (unsigned long i = 0; i < final->size(); i++)
	{
		if ((*final)[i].value == value)
			return i;
	}
	int result = final->size() - 1;
	return result >= 0 ? result : 0;
}

static std::list<Number>::iterator rightIndexL(std::list<Number> *final, int value)
{
	for (std::list<Number>::iterator it = final->begin(); it != final->end(); ++it)
	{
		if (it->value == value)
			return it;
	}
	std::list<Number>::iterator result = final->end();
	if (result != final->begin())
		--result;
	return result;
}

static void binaryInsert(std::vector<Number> *final, const Number& n){
	int left = 0;
	int right =  rightIndex(final, n.pair);
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

static void binaryInsertL(std::list<Number> *final, const Number& n){
	std::list<Number>::iterator left = final->begin();
	// std::cout << "value = " << n.value << " and final content = ";
	// for(std::list<Number>::iterator it = final->begin(); it != final->end(); ++it){
	// 	std::cout << it->value << " ";
	// }
	// std::cout << std::endl;
	std::list<Number>::iterator right = rightIndexL(final, n.pair);
	std::list<Number>::iterator mid;
	std::list<Number>::iterator tmp;
	// std::cout << "left value = " << left->value << " and right value = " << right->value << std::endl;
	if (final->size() == 1)
	{
		if ((*final).front().value > n.value)
			final->insert(final->begin(), n);
		else
			final->push_back(n);
	}
	else if (n.value < (*final).front().value)
	{
		final->insert(final->begin(), n);
	}
	else
	{
		// while (std::distance(final->begin(), left) <= std::distance(final->begin(), right)) //(left->value != right->value && left->pair != right->pair) && 
		// {
		// 	mid = left;
		// 	std::advance(mid, std::distance(left, right) / 2);
		// 	tmp = mid;
		// 	if (mid->value < n.value) {
				
		// 		std::advance(tmp, 1);
		// 		left = tmp;
		// 	} else {
		// 		std::advance(tmp, -1);
		// 		right = tmp;
		// 	}
		// }
		while (std::distance(final->begin(), left) <= std::distance(final->begin(), right)) {
        mid = left;
        advance(mid, std::distance(left, right) / 2);

        if (mid->value == n.value) {
            // Insérer après cet élément s'il y a des éléments avec la même valeur
            ++mid;
            final->insert(mid, n);
            return;
        } else if (mid->value < n.value) {
            left = mid;
            ++left;
        } else {
            right = mid;
            --right;
        }
    }
		// std::list<Number>::iterator index = final->begin();
		// std::advance(index, std::distance(final->begin(), left));
		// if (n.value > index->value)
		// 	std::advance(index, 1);
		final->insert(left, n);
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
	jacobsthalIndex = 1;
	lPart(argv);
}

bool PmergeMe::vPart(char **argv){
	//Demarrer le chrono ici
	clock_t debut = clock();
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
	if (isSortedV(vSequence))
		return false;
	vCopy = vSequence;
	completePairsV();
	sortPairsV();
	if (isSortedV(vSequence)) // Check si la sequence est deja triee
	{
		clock_t fin = clock();
		finalDisplay(&vSequence, debut, fin);
		return true;
	}

	//Push les max dans un vector en commencant par le dernier element du vecteur
	n = vSequence[vSequence.size()-1];
	final.push_back(n);
	vSequence.pop_back();
	for(unsigned long i = 1; i < vSequence.size(); i+=2){
		if (i % 2 != 0){
			binaryInsert(&final, vSequence[i]);
		}
	}

	// // Supprimer les elements qui ont ete push dans final 
	for(std::vector<Number>::iterator it = vSequence.begin(); it < vSequence.end(); ++it){
		if (it->value > it->pair)
			vSequence.erase(it);
	}

	// // On commence par insérer le Number qui est lié au 1er Number de final
	for(std::vector<Number>::iterator it = vSequence.begin(); it < vSequence.end(); ++it){
		if (it->value == final.front().pair)
		{
			final.insert(final.begin(), *it);
			vSequence.erase(it);
			break;
		}
	}
	// Puis on insert le reste des Number dans final
	while (!vSequence.empty())
	{
		unsigned long subsize = nextJacobsthal(0, 2, 0);
		if (subsize > vSequence.size())
			subsize = vSequence.size();
		jacobsthalIndex++;
		for (int i = subsize - 1; i >= 0; i--)
		{
			binaryInsert(&final, vSequence[i]);
			vSequence.erase(vSequence.begin() + i);
		}
	}
	clock_t fin = clock();
	finalDisplay(&final, debut, fin);
	return true;
}


bool PmergeMe::isSortedV(std::vector<Number>& vec){
	for(unsigned long i = 0; i < vec.size() - 1; i++){
		if (vec[i].value > vec[i + 1].value)
			return false;
	}
	return true;
}

bool PmergeMe::isSortedL(std::list<Number>& li){
	std::list<Number>::iterator next_it;
	for(std::list<Number>::iterator it = li.begin(); it != --li.end(); ++it){
		next_it = it;
		++next_it;
		if (it->value > next_it->value)
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

void PmergeMe::completePairsL(){
	unsigned long i = 0;
	std::list<Number>::iterator next_it;
	for (std::list<Number>::iterator it = lSequence.begin(); it != lSequence.end(); ++it)
	{
		next_it = it;
		if (i % 2 == 0 && (i + 1) < lSequence.size())
		{
			++next_it;
			it->pair = next_it->value;
		}
		else if ((i + 1) < lSequence.size() || lSequence.size() % 2 == 0)
		{
			--next_it;
			it->pair = next_it->value;
		}
		else
			it->pair = -1;
		i++;
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

void PmergeMe::sortPairsL(){
	int tmpV;
	int tmpP;
	std::list<Number>::iterator next_it;
	std::list<Number>::iterator it = lSequence.begin();
	while (it != lSequence.end())
	{
		tmpV = it->value;
		tmpP = it->pair;
		next_it = it;
		++next_it;
		if (next_it == lSequence.end())
			break;
		if (tmpV > next_it->value)
		{
			it->value = next_it->value;
			it->pair = next_it->pair;
			next_it->value = tmpV;
			next_it->pair = tmpP;
		}
		std::advance(it, 2);
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

void PmergeMe::finalDisplay(std::vector<Number> *sequence, clock_t debut, clock_t fin){
	std::cout << "Before: ";
	displayContent(vCopy);
	double timelapse = (double)(fin - debut) * 1000.0 / (double) CLOCKS_PER_SEC;
	std::cout << "After: ";
	displayContent(*sequence);
	std::cout << "Time to process a range of " << sequence->size() <<" elements with std::vector : " << timelapse  << " milliseconds" << std::endl;
}


bool PmergeMe::lPart(char **argv){
	//Demarrer le chrono ici
	clock_t debut = clock();
	std::list<std::string> test;
	std::list<Number> final;
	Number n;
	int i = 1;
	while (argv[i])
	{
		std::string tmp = argv[i];
		test = ft_split_list(tmp, ' ');
		for(std::list<std::string>::iterator it = test.begin(); it != test.end(); ++it){
			n.value = atoi(it->c_str());
			lSequence.push_back(n);
		}
		i++;
	}
	completePairsL();
	sortPairsL();
	if (isSortedL(lSequence)) // Check si la sequence est deja triee
	{
		clock_t fin = clock();
		double timelapse = (double)(fin - debut) * 1000.0 / (double) CLOCKS_PER_SEC;
		std::cout << "Time to process a range of " << lSequence.size() <<" elements with std::vector : " << timelapse  << " milliseconds" << std::endl;
		return true;
	}

	// //Push les max dans un vector en commencant par le dernier element du vecteur
	n = lSequence.back();
	final.push_back(n);
	lSequence.pop_back();
	
	std::list<Number>::iterator it = lSequence.begin();
	std::advance(it, 1);
	int j = 1;
	while (it != lSequence.end())
	{
		if (j % 2 != 0)
			binaryInsertL(&final, *it);
	
		std::advance(it, 1);
		if (it == lSequence.end())
			break;
		std::advance(it, 1);
		if (it == lSequence.end())
			break;
		j+= 2;
	}
	// Supprimer les elements qui ont ete push dans final 
	std::list<Number>::iterator itl = lSequence.begin();
	while (itl != lSequence.end()) 
	{
		if (itl->value > itl->pair)
			itl = lSequence.erase(itl);
		else
			++itl;
	}

	// // // On commence par insérer le Number qui est lié au 1er Number de final
	for(std::list<Number>::iterator it = lSequence.begin(); it != lSequence.end(); ++it){
		if (it->value == final.front().pair)
		{
			final.insert(final.begin(), *it);
			lSequence.erase(it);
			break;
		}
	}

	std::list<Number>::iterator subIt = lSequence.begin();
	while (!lSequence.empty())
	{
		unsigned long subsize = nextJacobsthal(0, 2, 0);
		if (subsize > lSequence.size())
			subsize = lSequence.size();
		jacobsthalIndex++;
		std::advance(subIt, (subsize - 1));
		for (int i = subsize; i > 0; i--)
		{
			binaryInsertL(&final, *subIt);
			if (subIt == lSequence.begin())
				break;
			subIt--;
		}
		subIt = lSequence.begin();
		for (unsigned long i = 0; i < subsize; i++)
		{
			lSequence.erase(subIt);
			subIt = lSequence.begin();
		}
	}
	clock_t fin = clock();
	double timelapse = (double)(fin - debut) * 1000.0 / (double) CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " << final.size() <<" elements with std::list : " << timelapse  << " milliseconds" << std::endl;
	return true;
}