#include "util.h"
char encode10to2(string s){
    bitset<8> bit(s);
    char res = bit.to_ulong();
    return res;
}