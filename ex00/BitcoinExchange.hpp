#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include <iomanip>
# include <string>
# include <map>

# define BIS 29
# define NOTBIS 28


class BitcoinExchange{
	public:

		BitcoinExchange(std::ifstream& db);
		~BitcoinExchange();
		void convert(std::ifstream& infile) const;

	private:

		std::map<std::string, float, std::greater<std::string> > data;


		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange & other);
		BitcoinExchange& operator=(const BitcoinExchange & other);
		void extract_data(std::ifstream& db);
		void parseAndConvert(std::string &line) const;
		void convertData(std::string& date, std::string& value) const;
};

#endif