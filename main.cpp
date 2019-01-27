#include <iostream>
#include <string>
#include <vector>

#include "instance.h"

int main(int argc, char* argv[])
{
	if (argc < 3) return 1; //not enough arguments

	std::vector<std::string> argv_vec;
	argv_vec.push_back(std::string(argv[1])); //player side
	argv_vec.push_back(std::string(argv[2])); //input file name
	argv_vec.push_back(std::string(argv[3])); //output file name

	Instance instance(argv_vec);
	instance.Run();

#ifdef _DEBUG
	system("PAUSE");
#endif // _DEBUG

	return 0;
}