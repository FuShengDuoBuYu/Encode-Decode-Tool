#ifndef FILEIO_H
#define FILEIO_H
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <bitset>
#include "util.h"
using namespace std;
// 每个文件的头部,包含文件的基信息
struct fileHead{
    //字符种类
    int alphaVarity;
    //最后一个字节的有效位数
    int lastValidBit;
};
//字母及其频度
struct alphaCode{
    char alpha;
    string code;
    alphaCode(){}
    alphaCode(const pair<char ,string>& x){
        this->alpha = x.first;
        this->code = x.second;
    }
};
class FileIO{
    public:
        string sourceFileName;
        string desFileName;
        //压缩文件的方法
        void encodeFile(string desFileName,map<char, string> charCode);
        map<char, long long> getCharFreq();
        FileIO(string sourceFileName,string desFileName){
            this->sourceFileName = sourceFileName;
            this->desFileName = desFileName;
        }
        
        
    private:
        
};


#endif