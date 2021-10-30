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
        map<char, long long> getCharFreq();
        FileIO(string sourceFileName,string desFileName){
            this->sourceFileName = sourceFileName;
            this->desFileName = desFileName;
        }
    private:
        string sourceFileName;
        string desFileName;
};

#endif