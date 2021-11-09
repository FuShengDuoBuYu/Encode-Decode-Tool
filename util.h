#include <string>
#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
char encode10to2(string s);
int encode2to10(string s);
string encode10to2(int length, int value);
int getEncodeOrDecode();
vector<string> getEncodeName();
vector<string> getDecodeName();