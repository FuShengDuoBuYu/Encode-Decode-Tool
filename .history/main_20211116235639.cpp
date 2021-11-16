#include "fileIO.h"
#include "Haffman.h"

long long encodeSingleFile(string sourceFilename,string desFilename,int writeMode){
    long long size;
    //判断是否存在目标文件
    path str(desFilename);
    directory_entry entry(str);
    //如果无目标文件,则其大小就是0
    if(entry.status().type() == file_type::not_found)
        size = 0;
    else
        size = file_size(str);

    FileIO fileIO= FileIO(sourceFilename,desFilename);
    //字符的频率map
    map<char, long long> charFreq = fileIO.getCharFreq();
    Haffman haffman = Haffman(charFreq);
    //字符的哈夫曼编码map
    map<char, string> charCode = haffman.createHaffmanCode();
    //根据哈夫曼编码来将文件压缩
    fileIO.encodeFile(fileIO.desFileName,charCode,charFreq,writeMode);
    //返回写文件的大小
    return file_size(str)-size;
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

void encodeDir(string path,string desFilename){
    ofstream out(desFilename);
    vector<string> fileName;
    vector<string> dirName;
    for(auto const& entry: recursive_directory_iterator(path)){
        if(entry.status().type() == file_type::directory){
            dirName.push_back(entry.path().string());
        }else{
            fileName.push_back(entry.path().string());
        }
    }
    //写入目录文件
    out << dirName.size() << endl;
    for (int i = 0; i < dirName.size();i++){
        out << dirName[i] << endl;
    }
    //写入文件文件
    out << fileName.size() << endl;
    for (int i = 0; i < fileName.size();i++){
        //写文件名
        out << fileName[i] << endl;
    }
    out.close();
    //写各个二进制文件
    vector<long long> afterSizes;
    for (int i = 0; i < fileName.size(); i++){
        int aftersize = encodeSingleFile(fileName[i], desFilename, 1);
        afterSizes.push_back(aftersize);
    }
    //写压缩后的大小
    ofstream fout(desFilename,ios::app);
    out << "\n";
    for (int i = 0; i < afterSizes.size();i++){
        fout << " " << afterSizes[i] ;
    }
}

void decodeDir(string sourceFilename,string desFilename){
    ifstream is(sourceFilename);
    string dirNum,filesNum,str_filesize;
    string path;
    //获取文件夹目录
    getline(is,dirNum);
    int fileNum =  atoi(dirNum.c_str());
    for (int i = 0; i < fileNum;i++){
        getline(is,path);
        create_directories(path);
    }
    //获取文件
    getline(is,filesNum);
    int fileNums =  atoi(filesNum.c_str());
    //文件路径
    vector<string> filePaths;
    //文件大小
    vector<string> filesize;
    for (int i = 0; i < fileNums;i++){
        getline(is,path);
        filePaths.push_back(path);
        getline(is, str_filesize);
        filesize.push_back(str_filesize);
    }
    int startIndex = is.tellg();
    is.clear();
    is.close();
    vector<long long> aftersize = getAfterSize(sourceFilename,filesize.size());
    //解压各个分文件

    ifstream is2(sourceFilename, ios::binary);
    is2.seekg(startIndex);
    for (int i = 0; i < filesize.size();i++){
        ofstream temp("temp.hfm",ios::binary);
        //将每个文件的二进制数据写到temp里
        char buffer;
        for (long long j = 0; j < aftersize[i];j++){
            is2.read(&buffer, sizeof(char));
            temp.write(&buffer, sizeof(char));
        }
        decodeSingleFile("temp.hfm",desFilename+"\\"+filePaths[i]);
        remove("temp.hfm");
    }
}

int main(int argc,char *argv[]){
    int mode = getEncodeOrDecode();
    switch(mode){
        case 1:{
            vector<string> filename = getEncodeName();
            cout << "Encoding , please wait a moment..." << endl;
            //文件夹的压缩
            if(filename[0]=="0"){
                encodeDir(filename[1],filename[2]);
            }
            //单个文件的压缩
            else{
                encodeSingleFile(filename[1], filename[2],0);
            }
            cout << "Encoding task finish!" << endl;
            break;
        }
        case 2:
            vector<string> filename = getDecodeName();
            cout << "Decoding , please wait a moment..." << endl;
            //文件夹解压
            if(filename[0]=="0"){
                decodeDir(filename[1], filename[2]);
            }
            //文件解压
            else{
                decodeSingleFile(filename[1], filename[2]);
            }
            cout << "Decoding task finish!" << endl;
    }
    return 0;
}
    



