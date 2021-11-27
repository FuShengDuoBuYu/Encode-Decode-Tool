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
#include "Haffman.h"
using namespace std;
// 每个文件的头部,包含文件的基信息
struct fileHead{
    //字符种类
    int alphaVarity;
    //源文件的字节数
    long long originBytes;
};
//字母及其频度
struct alphaCode{
    char alpha;
    //频度
    long long freq;
    alphaCode(){}
    alphaCode(const pair<char ,long long>& x){
        this->alpha = x.first;
        this->freq = x.second;
    }
};
//字母及其哈夫曼码
struct CharInfo{
		CharInfo() : code("") { }
		string code;
	}charInfoArray[256];

class FileIO{
    public:
        string sourceFileName;
        string desFileName;
        //压缩文件的方法
        void encodeFile(string desFileName,map<char, string> charCode,map<char, long long> charFreq,int writeMode);
        map<char, long long> getCharFreq();
        FileIO(string sourceFileName,string desFileName){
            this->sourceFileName = sourceFileName;
            this->desFileName = desFileName;
        }
        //获取最后一个字节的有效位数的方法
        int getLastValidBit(map<char, long long> charFreq, map<char, string> charCode);


        //读取一个文件中的头信息的方法
        fileHead readFileHead();
        //获取文件原char和频率的信息以恢复哈夫曼树
        map<char, long long> readFileHaffmanFreq(int alphaVariety);
        //解压缩文件
        void decodeFile(fileHead filehead,map<char, long long> decodeHaffmanFreq);

    private:
};


#endif