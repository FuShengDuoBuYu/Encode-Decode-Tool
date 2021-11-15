#include "fileIO.h"
#include "Haffman.h"

long long encodeSingleFile(string sourceFilename,string desFilename,int writeMode){
    FileIO fileIO= FileIO(sourceFilename,desFilename);
    //字符的频率map
    map<char, long long> charFreq = fileIO.getCharFreq();
    Haffman haffman = Haffman(charFreq);
    //字符的哈夫曼编码map
    map<char, string> charCode = haffman.createHaffmanCode();
    //根据哈夫曼编码来将文件压缩
    fileIO.encodeFile(fileIO.desFileName,charCode,charFreq,writeMode);
    //返回写文件的大小
    return file_size(path(desFilename));
}

void decodeSingleFile(string sourceFilename,string desFilename){
    FileIO fileIO= FileIO(sourceFilename,desFilename);
    //获取头信息
    fileHead filehead = fileIO.readFileHead();
    //获取对应文件的字符与频度
    map<char, long long> charFreq= fileIO.readFileHaffmanFreq(filehead.alphaVarity);  
    //解压缩文件
    fileIO.decodeFile(filehead,charFreq);
}

void encodeDir(string path){
    long long size = 0;
    for(auto const& dir_entry: directory_iterator(path)){
        size += encodeSingleFile(dir_entry.path().string(), "test.hfm",1);   
    }
}

int main(int argc,char *argv[]){
    string p;
    cin >> p;
    path str(p);
    directory_entry entry(str);
    if(entry.status().type() == file_type::directory){
        encodeDir(p);
    }
    else if(entry.status().type() == file_type::not_found){
        cout << "not found" << endl;
    }
    else{
        cout << file_size(p) << endl;
        cout << "is file" << endl;
    }
    // int mode = getEncodeOrDecode();
    // switch(mode){
    //     case 1:{
    //         vector<string> filename = getEncodeName();
    //         cout << "Encoding , please wait a moment..." << endl;
    //         long long size = encodeSingleFile(filename[0], filename[1]);
    //         cout << "Encoding task finish!" << endl;
    //         break;
    //     }
    //     case 2:
    //         vector<string> filename = getDecodeName();
    //         cout << "Decoding , please wait a moment..." << endl;
    //         decodeSingleFile(filename[0], filename[1]);
    //         cout << "Decoding task finish!" << endl;
    // }
    // return 0;
}
