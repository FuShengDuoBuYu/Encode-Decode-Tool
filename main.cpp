#include "fileIO.h"
#include "Haffman.h"
int main(int argc,char *argv[]){
    int mode = getEncodeOrDecode();
    switch(mode){
        case 1:{
            FileIO fileIO= FileIO("b.exe","b.hfm");
            //字符的频率map
            map<char, long long> charFreq = fileIO.getCharFreq();
            Haffman haffman = Haffman(charFreq);
            //字符的哈夫曼编码map
            map<char, string> charCode = haffman.createHaffmanCode();
            //根据哈夫曼编码来将文件压缩
            fileIO.encodeFile(fileIO.desFileName,charCode,charFreq);
            break;
        }
        case 2:
            FileIO fileIO= FileIO("b.hfm","b.exe");
            //获取头信息
            fileHead filehead = fileIO.readFileHead();
            //获取对应文件的decodeHaffmanCode
            map<string,char> codeChar= fileIO.readFileHaffmanString(filehead.alphaVarity);
            cout << codeChar.size() << endl;
    }
    
    return 0;
}

