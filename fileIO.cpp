#include "fileIO.h"
// using namespace std;
map<char, long long> FileIO::getCharFreq(){
    //获取要输入的文件和输出的文件
    //用二进制流传输
    ifstream fin(sourceFileName,ios::binary);
    //用一个map匹配字符和其出现的频度
    map<char, long long> charFreq;
    while(!fin.eof()){
        //每次只接收一个字符,于是会有2^8种字符
        char buffer[1];
        fin.read(buffer, sizeof(char));
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
    fin.close();
    return charFreq;
}

void FileIO::encodeFile(string desFileName,map<char, string> charCode,map<char, long long> charFreq){
    ofstream fout(desFileName, ios::binary);
    //先将文件的头信息写好
    fileHead filehead;
    // 获取字符的种类,写头信息
    filehead.lastValidBit = getLastValidBit(charFreq, charCode);
    filehead.alphaVarity = charCode.size();
    fout.write((char *)&filehead, sizeof(filehead));
    //写字符的频度等等
    for(auto i:charCode){
        alphaCode af(i);
        fout.write((char*)&af,sizeof(af));
    }
    //写文件的内容
    ifstream fin(sourceFileName,ios::binary);
    string bitsToWrite = "";
    while(!fin.eof()){
        //每次只接收一个字符,于是会有2^8种字符
        char buffer[1];
        fin.read(buffer, sizeof(char));
        //获取此字符对应的哈夫曼编码
        string codeOfAlpha = charCode[buffer[0]];
        //将该字符串放到要写入的bits里
        bitsToWrite += codeOfAlpha;
        //一旦字符串长度大于8,就将其转为一个char并写入文件
        while (bitsToWrite.length() >= 8){
            char towrite = encode10to2(bitsToWrite.substr(0, 8));
            fout.write(&towrite, sizeof(char));
            bitsToWrite = bitsToWrite.substr(8, bitsToWrite.length() - 8);

        }
    }
    //将最后不足8位的bit补全并写入
    bitsToWrite.resize(8,'0');
    char towrite = encode10to2(bitsToWrite);
    fout.write(&towrite, sizeof(char));
    fout.close();
}

int FileIO::getLastValidBit(map<char, long long> charFreq,map<char, string> charCode){
    int sum = 0;
    for(auto i: charCode){
        sum += i.second.size() * charFreq.at(i.first);
        sum &= 0xFF;
    }
    sum &= 0x7;
    return sum == 0 ? 8 : sum;
}

fileHead FileIO::readFileHead(){
    fileHead filehead;
    ifstream is(sourceFileName, ios::binary);
    is.read((char *)&filehead, sizeof(filehead));
    is.close();
    return filehead;
}

map<string, char> FileIO::readFileHaffmanString(int alphaVariety){
    ifstream is(sourceFileName, ios::binary);
    map<string, char> codeChar;
    //定位在头信息后
    is.seekg(sizeof(fileHead));
    //将各个字符的编码string获取
    for (int i = 0; i < alphaVariety;i++){
        alphaCode af;
        is.read((char *)&af, sizeof(af));
        codeChar.insert(make_pair(encode10to2(af.length,af.code),af.alpha));
    }
    is.close();
    return codeChar;
}

void FileIO::decodeFile(fileHead filehead,map<string,char> decodeHaffmanCode){
    ifstream is(sourceFileName, ios::binary);
    ofstream out(desFileName, ios::binary);
    //定位到存储文件的位置
    is.seekg(sizeof(filehead) + filehead.alphaVarity * sizeof(alphaCode));
    //开始读取
    string encodedBitsBuffer = "";
    bool deleted = false;
    while(!is.eof()){
        //先读够足够的缓冲
        while(encodedBitsBuffer.length()<256&&!is.eof()){
            char tempToRead;
            is.read(&tempToRead, sizeof(char));
            bitset<8> bit(tempToRead);
            encodedBitsBuffer += bit.to_string();
        }
        // if(is.eof())
        map<string , char>::iterator it;
        //匹配是否存在某个字符串
        for (int i = 1;;i++){
            string compare = encodedBitsBuffer.substr(0, i);
            it = decodeHaffmanCode.find(compare);
            if(it!=decodeHaffmanCode.end()){
                out.write(&decodeHaffmanCode[compare], sizeof(char));
                encodedBitsBuffer = encodedBitsBuffer.substr(i, encodedBitsBuffer.length() - i);
                break;
            }
        }
    }
}