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
#include <stack>
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
    //将0101字符串转为int的十进制存储
    int code;
    //2进制0101字符串的长度
    int length;
    alphaCode(){}
    alphaCode(const pair<char ,string>& x){
        this->alpha = x.first;
        this->code = encode2to10(x.second);
        this->length = x.second.length();
    }
};
class FileIO{
    public:
        string sourceFileName;
        string desFileName;
        //压缩文件的方法
        void encodeFile(string desFileName,map<char, string> charCode,map<char, long long> charFreq);
        map<char, long long> getCharFreq();
        FileIO(string sourceFileName,string desFileName){
            this->sourceFileName = sourceFileName;
            this->desFileName = desFileName;
        }
        //获取最后一个字节的有效位数的方法
        int getLastValidBit(map<char, long long> charFreq, map<char, string> charCode);


        //读取一个文件中的头信息的方法
        fileHead readFileHead();
        //获取编码后的char的string并形成一个map
        map<string, char> readFileHaffmanString(int alphaVariety);
        //解压缩文件
        void decodeFile(fileHead filehead,map<string,char> decodeHaffmanCode);

    private:
};


#endif