#include "fileIO.h"
#include "Haffman.h"
int main(int argc,char *argv[]){
    FileIO fileIO= FileIO("b.exe","test.hfm");
    //字符的频率map
    map<char, long long> charFreq = fileIO.getCharFreq();
    Haffman haffman = Haffman(charFreq);
    //字符的哈夫曼编码map
    map<char, string> charCode = haffman.createHaffmanCode();
//     map<char, string>::iterator it;
//     map<char, long long>::iterator it2;
// // for(; it != charFreq.end(); it++){
// // cout << it->second << "\n"; //first 是key , second 是 value
// // }
//     for(it2 = charFreq.begin(),it = charCode.begin(); it != charCode.end(); it2++,it++){
//     cout << it2->second << " " << it->second << "\n";
//     }
    //根据哈夫曼编码来将文件压缩
    fileIO.encodeFile(fileIO.desFileName,charCode);
    return 0;
}