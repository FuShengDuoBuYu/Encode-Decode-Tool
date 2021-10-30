#include "fileIO.h"
#include "Haffman.h"
int main(int argc,char *argv[]){
    FileIO fileIO= FileIO("b.exe","test.exe");
    // map<char, long long> charFreq;
    //字符的频率map
    map<char, long long> charFreq = fileIO.getCharFreq();
    Haffman haffman = Haffman(charFreq);
    //字符的哈夫曼编码map
    map<char, string> charCode = haffman.createHaffmanCode();
    return 0;
}