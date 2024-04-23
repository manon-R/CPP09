#include "BitcoinExchange.hpp"

using std::ifstream;
using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if (argc != 2 || !argv[1][0])
	{
		cout << "One argument required (database file)" << endl;
		return 0;
	}
	ifstream infile(argv[1]);
	if (!infile)
	{
		cout << "Error: Impossible to open the file " << argv[1] << endl;
		return 0;
	}
	if (infile.peek() == std::ifstream::traits_type::eof())
	{
		cout << "Error: " << argv[1] << " file is empty" << endl;
		return 0;
	}
	ifstream db("data.csv");
	if (!db)
	{
		cout << "Error: Impossible to open the file data.csv" << endl;
		return 0;
	}
	if (db.peek() == std::ifstream::traits_type::eof())
	{
		cout << "Error: csv file is empty" << endl;
		return 0;
	}

	
	BitcoinExchange be(db);
	be.convert(infile);


	db.close();
	infile.close();
}