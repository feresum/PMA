Programming language for 2-dimensional pattern matching.

For more information, see the [documentation page](https://github.com/feresum/PMA/blob/master/doc.md).

Supported compilers: gcc 4.7, Microsoft VC++ 18.0. 

<h5>To build:</h5>

One may either use the CMake build system, or run one of the following commands:

    g++ -std=c++11 -DNDEBUG -O2 snails/*.cpp main/*.cpp -o snails

or

    cl /nologo /EHsc /DNDEBUG /O2 snails/*.cpp main/*.cpp /Fesnails

<h5>Usage:</h5>

    snails <program file> <input file>
