#include "Threads.h"
#include "IteratorCSV.h"
#include "File_identifier.h"
#include <map>

int main()
{
	std::map<std::string, algorithm*> lol;
	Identifier("file_of_files.csv", lol);
	paralel_alg<2>(lol);
	system("pause");
	return 0;
}