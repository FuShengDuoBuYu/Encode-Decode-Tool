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

void encodeDir(string path){
    long long size = 0;
    vector<fileNode> filenode;
    for(auto const& dir_entry: directory_iterator(path)){
        fileNode node(1,encodeSingleFile(dir_entry.path().string(), "test.hfm",1));
        filenode.push_back(node);
    }
    
}
void decodeDir(string path){

}
//将文件目录写到文件中,返回文件名
vector<string> writeDirToFile(string path,string desFilename){
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
        //写文件大小
        out << file_size(fileName[i]) << endl;
    }
    out.close();
    return fileName;
}
//读取压缩完后各个文件的大小
vector<long long> getAfterSize(string path,int nums){

    ifstream fin("test.hfm", ios::ate);
    fin.seekg(-1, fin.cur);
    // 查看前一个字符是否为回车符
    while ((char)fin.peek() != '\n'){
        fin.seekg(-1, fin.cur);
    }
    fin.seekg(2, fin.cur);
    string line;
    getline(fin, line);
    fin.clear();
    fin.close();
    vector<long long> res;
    istringstream istr(line);
    for (int i = 0; i < nums;i++){
        string temp;
        istr >> temp;
        res.push_back(atoll(temp.c_str()));
    }
    return res;
}
//将文件目录还原,返回是文件的目录
vector<vector<string>> CreateDirFromFile(){
    ifstream is("test.hfm");
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
    vector<long long> aftersize = getAfterSize("test.hfm",filesize.size());
    //解压各个分文件

    ifstream is2("test.hfm", ios::binary);
    is2.seekg(startIndex);
    for (int i = 0; i < filesize.size();i++){
        ofstream temp("temp.hfm",ios::binary);
        //将每个文件的二进制数据写到temp里
        char buffer;
        // int readSize = 0;
        for (long long j = 0; j < aftersize[i];j++){
            is2.read(&buffer, sizeof(char));
            temp.write(&buffer, sizeof(char));
        }
        decodeSingleFile("temp.hfm",filePaths[i]);
        remove("temp.hfm");
    }
    // ifstream is2("test.hfm", ios::binary);
    // is2.seekg(808);
    // for (int i = 0; i < 1;i++){
    //     ofstream temp("temp.hfm",ios::binary);
    //     //将每个文件的二进制数据写到temp里
    //     char buffer;
    //     // int readSize = 0;
    //     for (long long j = 0; j < 10000;j++){
    //         is2.read(&buffer, sizeof(char));
    //         temp.write(&buffer, sizeof(char));
    //     }
    // }
    vector<vector<string>> res;
    res.push_back(filePaths);
    res.push_back(filesize);
    is.close();
    return res;
}


// void decodeDirFile(vector<string> filePath,vector<long long> filesize){
//     // ifstream is("test.hfm");
//     // string flag;
//     // int startIndex;
//     // is.close();
//     // cout << startIndex << endl;
//     // ifstream is2("test.hfm", ios::binary);
//     // is2.seekg(startIndex);
//     // ofstream out("copy", ios::binary);
//     // char buff[2000000];
//     // is2.read(buff, 135220);
//     // out.write(buff, 135220);
//     // out.close();

// }






int main(int argc,char *argv[]){
    vector<vector<string>> fileInfo = CreateDirFromFile();
    vector<string> filePaths = fileInfo[0];
    vector<string> str_filesize = fileInfo[1];
    //将文件大小转为longlong
    vector<long long> filesize;
    for (int i = 0; i < str_filesize.size();i++){
        filesize.push_back(atoll(str_filesize[i].c_str()));
    }
    //依次解压各个文件
    // decodeDirFile(filePaths,filesize);

    // vector<string> filename = writeDirToFile("folder\\3","test.hfm");
    // vector<long long> afterSizes;
    // for (int i = 0; i < filename.size(); i++){
    //     int aftersize = encodeSingleFile(filename[i], "test.hfm", 1);
    //     afterSizes.push_back(aftersize);
    // }
    // ofstream out("test.hfm",ios::app);
    // out << "\n";
    // for (int i = 0; i < afterSizes.size();i++){
    //     out << " " << afterSizes[i] ;
    // }
    // string p;
    // cin >> p;
    // path str(p);
    // directory_entry entry(str);
    // if(entry.status().type() == file_type::directory){
    //     // int i;
    //     // cin >> i;
    //     // if(i = 0)
    //     // writeDirToFile(p);
    //     CreateDirFromFile();
    //     // encodeDir(p);
    //     // else if(i = 1){
    //     //     decodeDir(p);
    //     // }
    // }
    // else if(entry.status().type() == file_type::not_found){
    //     cout << "not found" << endl;
    // }
    // else{
    // int mode = getEncodeOrDecode();
    // switch(mode){
    //     case 1:{
    //         vector<string> filename = getEncodeName();
    //         cout << "Encoding , please wait a moment..." << endl;
    //         path p(filename[0]);
    //         directory_entry entry(p);
    //         //文件夹的压缩
    //         if(entry.status().type() == file_type::directory){
    //             writeDirToFile(filename[0],filename[1]);

    //         }
    //         //单个文件的压缩
    //         else{
    //             encodeSingleFile(filename[0], filename[1],0);
    //         }
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
    



