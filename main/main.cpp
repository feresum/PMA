#include <sstream>
#include <fstream>
#include <iostream>
#include "../snails/interpreter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Required arguments: <source file> <input file>\n";
        return 1;
    }
    char *srcfile = argv[1], *infile = argv[2];

    std::ifstream srcfs(srcfile);
    std::stringstream srcss;
    srcss << srcfs.rdbuf();
    srcfs.close();
    if (srcfs.fail()) {
        std::cerr << "Couldn't read source file: " << srcfile;
        return 1;
    }

    std::ifstream infs(infile);
    std::ostringstream inss;
    inss << infs.rdbuf();
    infs.close();
    if (infs.fail()) {
        std::cerr << "Couldn't read input file: " << infile;
        return 1;
    }

    interpreter(srcss.str(), inss.str(), std::cout, std::cerr);

    return 0;
}

