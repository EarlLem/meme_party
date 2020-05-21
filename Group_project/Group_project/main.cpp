#include "linear_alg.h"
#include "RKmethod.h"
#include "Threads.h"
#include "File_identifier.h"
#include <map>
#include <iostream>
#include "CSVReader.h"

int main()
{
	map<string, algorithm*> task;
	task["files/test1.csv"] = new RKintegrator<RKmethod_5_4_7fs, Osc_Van_der_Pol>;
	task["files/test2.csv"] = new RKintegrator<VernerRK_5_6_8, LVOde>;
	paralel_alg<1>(task);
	system("pause");
	return 0;
}