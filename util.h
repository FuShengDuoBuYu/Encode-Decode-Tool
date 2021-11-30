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
//读取压缩完后各个文件的大小
vector<long long> getAfterSize(string path, int nums);