
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../snails/interpreter.h"

const std::string TYPES("pioe");
const char BLOCK_O = '[', BLOCK_C = ']', LINE_O = '=';

int main(int argc, char* argv[])
{
    const char* fname = argc > 1 ? argv[1] : "tests.txt";

    std::ifstream fs;
    

    std::vector<std::string> pr, in;
    std::string ln, str;
    int line = 0;
#define READLN (++line, std::getline(fs, ln))
#define FMT_ASSERT(E) if(!(E)) { std::cerr << "Bad test file format at line " << line << std::endl; return 2; }
#define FAIL_TEST(MSG) std::cerr << "Test failed: " << MSG << " on test #" << ntests << " at line " << line <<std::endl; return 3

    try {
        fs.open(fname);
        int last = 0;
        while (fs.exceptions(std::ifstream::badbit), !fs.eof() && READLN) {
            fs.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
            int t, blk;
            if (ln.size() >= 2 && (t = 1 + TYPES.find(ln[0])) && ((blk = ln[1] == BLOCK_O) || ln[1] == LINE_O)) {
                if (t > 2) FMT_ASSERT(pr.size() && in.size());
                if (blk) {
                    FMT_ASSERT(!~ln.find_first_not_of(BLOCK_O, 2));
                    int nb = ln.size() - 1;
                    bool first = true;
                    str.clear();
                    while (READLN, ln.find_first_not_of(BLOCK_C) < nb || ln.size() < nb) {
                        if (first) first = false;
                        else str.append("\n");
                        str.append(ln);
                    }
                    FMT_ASSERT(ln.size() == nb);
                } else str = ln.substr(2);
                if (t == 1) {
                    if (last > 1) pr.clear();
                    pr.push_back(str);
                } else if (t == 2) {
                    if (last != 2) in.clear();
                    in.push_back(str);
                } else {
                    int ntests = 0;
                    for (std::string &p : pr) {
                        for (std::string &i : in) {
                            ++ntests;
                            std::stringstream out, err;
                            interpreter(p, i, out, err);
                            if (t == 3) {
                                if (!err.str().empty()) {
                                    FAIL_TEST("non-empty error output");
                                }
                                if (out.str() != str) {
                                    FAIL_TEST("incorrect output");
                                }
                            } else {
                                if (err.str().substr(str.size()) != str) {
                                    FAIL_TEST("incorrect error output");
                                }
                            }
                        }
                    }
                    std::cout << "Passed " << ntests << " tests on line " << line << std::endl;
                }
                last = t;
            }
        }
        FMT_ASSERT(!last || last > 2);

    } catch (std::ifstream::failure x) {
        std::cerr << "Error reading " << fname << std::endl;
        return 1;
    }

    std::cout << "All tests succeeded\n";

	return 0;
}

