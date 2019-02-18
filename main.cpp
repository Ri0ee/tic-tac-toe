#include <iostream>
#include <string>
#include <vector>

#include "instance.h"

int main(int argc, char* argv[])
{
#ifndef _DEBUG
	if (argc < 3) return 1; //not enough arguments
#endif // _DEBUG

	std::vector<std::string> argv_vec;
#ifdef _DEBUG
	argv_vec.push_back("0");
	argv_vec.push_back("field.in");
	argv_vec.push_back("field.out");
#else
	argv_vec.push_back(std::string(argv[1])); //player side
	argv_vec.push_back(std::string(argv[2])); //input file name
	argv_vec.push_back(std::string(argv[3])); //output file name
#endif // _DEBUG

	Instance instance(argv_vec);
	instance.Run();

#ifdef _DEBUG
	system("PAUSE");
#endif // _DEBUG

	return 0;
}