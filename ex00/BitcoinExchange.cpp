#include "BitcoinExchange.hpp"


static float castValue(const char* value){
	float floatValue;

	std::stringstream ss;
	ss << std::setprecision(3) << std::fixed << value;
	ss >> floatValue;
	return floatValue;
}

static bool checkValue(std::string& value){
	if (value.empty() || value[0] != ' ')
	{
		std::cout << "Error: bad line format (date | value)" << std::endl;
		return false;
	}
	std::string subValue = value.substr(1);
	std::string::iterator it;
	int nPoint = 0;
	for (it = subValue.begin(); it != subValue.end(); ++it){
		if ((*it < '0' || *it > '9') && *it != '.')
		{
			if (*it == '-')
				std::cout << "Error: not a positive number." << std::endl;
			else
				std::cout << "Error: bad value format => " << subValue << std::endl;
			return false;
		}
		if (*it == '.' && nPoint == 0)
			nPoint++;
		else if (*it == '.')
		{
			std::cout << "Error: bad value format => " << subValue << std::endl;
			return false;
		}
	}
	char* p_end;
	double testValue = std::strtod(subValue.c_str(), &p_end);
	if (testValue < 0 || testValue > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

static bool isValidDate(long year, long month, long day){
	int maxDay;
	if (year < 2009)
	{
		std::cout << "Error: Bitcoin didn't exist until 2009..." << std::endl;
		return false; 
	}
	if ((year == 2012 || year == 2016 || year == 2020 || year == 2024) && month == 2)
		maxDay = BIS;
	else if (month == 2)
		maxDay = NOTBIS;
	else if (month == 1 || month == 3 || month == 5 ||month == 7 ||month == 8 || month == 10 || month == 12)
		maxDay = 31;
	else
		maxDay = 30;
	if (day > maxDay)
	{
		std::cout << "Error: this date doesn't exist => " << year << "-" << month << "-" << day << std::endl;
		return false; 
	}
	return true;
}

//Fonction a reformater! Trop longue!! Faire des fonctions intermediaires
static bool checkDate(std::string& date){
	
	int i;
	for (i = 0;  i < 4; i++){
		if (!date[i] || date[i] < '0' || date[i] > '9')
		{
			std::cout << "Error: bad date format => " << date << std::endl;
			return false;
		}
	}
	std::string year = date.substr(0, 4);
	char* p_end;
	long testYear = std::strtol(year.c_str(), &p_end, 10);
	if (!date[i] || date[i] != '-')
	{
		std::cout << "Error: bad date format => " << date << std::endl;
		return false;
	}
	i++;
	for (int y = i; y < i+2; y++){
		if (!date[y] ||  date[y] < '0' || date[y] > '9')
		{
			std::cout << "Error: bad date format => " << date << std::endl;
			return false;
		}
	}
	std::string month = date.substr(5, 2);
	long testMonth = std::strtol(month.c_str(), &p_end, 10);
	if (testMonth < 1 || testMonth > 12)
	{
		std::cout << "Error: bad date format => " << date << std::endl;
		return false;
	}
	if (!date[7] || date[7] != '-')
	{
		std::cout << "Error: bad date format => " << date << std::endl;
		return false;
	}
	for (int y = 8; y < 10; y++){
		if (!date[y] ||  date[y] < '0' || date[y] > '9')
		{
			std::cout << "Error: bad date format => " << date << std::endl;
			return false;
		}
	}
	std::string day = date.substr(8, 2);
	long testDay = std::strtol(day.c_str(), &p_end, 10);
	if (testDay < 1 || testDay > 31)
	{
		std::cout << "Error: bad date format => " << date << std::endl;
		return false;
	}
	if (!date[10] || date[10] != ' ')
	{
		std::cout << "Error: bad line format (date | value)" << std::endl;
		return false;
	}
	return isValidDate(testYear, testMonth, testDay);
}

void BitcoinExchange::convertData(std::string& date, std::string& value) const{
	char *p_end;
	double testValue = std::strtod(value.c_str(), &p_end);
	double result = testValue * data.find(date)->second;
	std::cout << date << " => " << value << " = " << result << std::endl;
}

void BitcoinExchange::parseAndConvert(std::string &line) const{
	size_t pos = line.find('|');
	if (pos == line.npos)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}
	std::string date = line.substr(0, pos);
	std::string value = line.substr(pos+1);
	if (!checkDate(date) || !checkValue(value))
		return ;
	std::string finalDate = line.substr(0, pos-1);
	std::string finalValue = line.substr(pos+2);
	if (data.find(finalDate) != data.end())
		return convertData(finalDate, finalValue);
	else
		return convertData((std::string&)(data.upper_bound(finalDate)->first), finalValue);
}

BitcoinExchange::BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(std::ifstream& db){
	extract_data(db);
}

BitcoinExchange::~BitcoinExchange(){
	if (!data.empty())
		data.clear();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange & other){
	*this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange & other){
	if (!data.empty())
		data.clear();
	std::map<std::string, float>::iterator it;
	for (it = data.begin(); it != other.data.end(); ++it){
		data[it->first] = it->second;
	}
	return *this;
}

//parcours infile et convertis ou affiche un message d'erreur
void BitcoinExchange::convert(std::ifstream& infile) const{
	std::string line;
	while(std::getline(infile, line))
	{
		if (line == "date | value")// ignorer la premiere ligne
			continue;
		else
			parseAndConvert(line);
	}
}

void BitcoinExchange::extract_data(std::ifstream& db){
	std::string line;
	int i = 0;
	size_t pos;
	while(std::getline(db, line))
	{
		if (i == 0)// ignorer la premiere ligne date,exchange_rate
		{
			i++;
			continue;
		}
		pos = line.find(',');
		data[line.substr(0, 10)] = castValue(line.substr(pos+1).c_str());
	}
}


//display la map
// std::map<std::string, float>::const_iterator it;
// for (it = data.begin(); it != data.end(); ++it){
// 	std::cout << it->first << " = "  << std::fixed << std::setprecision(2)<< it->second << std::endl;
// }