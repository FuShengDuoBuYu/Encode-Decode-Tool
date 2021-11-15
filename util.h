#include <string>
#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
using namespace std;
using namespace std::filesystem;
char encode10to2(string s);
int encode2to10(string s);
string encode10to2(int length, int value);
int getEncodeOrDecode();
vector<string> getEncodeName();
vector<string> getDecodeName();

struct fileNode{
    //文件类型,0为文件夹,1为文件
    int type;
    //文件大小
    int filesize;
    fileNode(int type,int filesize){
        this->type = type;
        this->filesize = filesize;
    }
};