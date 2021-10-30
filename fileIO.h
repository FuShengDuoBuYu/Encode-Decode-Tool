#ifndef FILEIO_H
#define FILEIO_H
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;
class FileIO{
    public:
        void getCharFreq(string sourceFileName,string desFileName);
        FileIO(){}
};

#endif