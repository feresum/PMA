Programming language for 2-dimensional pattern matching.
Supported compilers: gcc 4.7, Microsoft VC++ 18.0. 

##To build:

    g++ -std=c++11 -O2 snails/*.cpp main/*.cpp -o snails
    
or

    cl /nologo /EHsc /O2 snails/*.cpp main/*.cpp /Fesnails

##Usage:
snails <program file> <input file>
