#include "fileIO.h"
// using namespace std;
map<char, long long> FileIO::getCharFreq(){
    //获取要输入的文件和输出的文件
    //用二进制流传输
    ofstream fout(desFileName,ios::binary);
    ifstream fin(sourceFileName,ios::binary);
    //用一个map匹配字符和其出现的频度
    map<char, long long> charFreq;
    while(!fin.eof()){
        //每次只接收一个字符,于是会有2^8种字符
        char buffer[1];
        fin.read(buffer, sizeof(char));
        fout.write(buffer, sizeof(char));
        //如果map里没有这个字符,就加入这个字符并将其频度设为1
        if(charFreq.count(buffer[0])==0){
            charFreq.insert(map<char, long long>::value_type(buffer[0], 1L));
        }
        //如果map中有这个字符,就将其频度++
        else{
            long long freq = charFreq[buffer[0]];
            freq++;
            charFreq[buffer[0]] = freq;
        }
    }
//     map<char, long long>::iterator it;
// for(it = charFreq.begin(); it != charFreq.end(); it++){
// cout <<  it->second << "\n"; //first 是key , second 是 value
// }
    //关闭文件流
    fout.close();
    fin.close();
    return charFreq;
}