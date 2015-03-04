#include <sstream>
#include <fstream>
#include <iostream>
#include "interpreter.h"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Arguments: <source file> <input file>\n";
        return 1;
    }
    char *srcfile = argv[1], *infile = argv[2];

    std::ifstream srcfs(srcfile);
    std::stringstream srcss;
    srcss << srcfs.rdbuf();
    srcfs.close();

    std::ifstream infs(infile);
    std::stringstream inss;
    inss << infs.rdbuf();
    infs.close();

    interpreter(srcss.str(), inss.str(), std::cout, std::cerr);

	return 0;
}

