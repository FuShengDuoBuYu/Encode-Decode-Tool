#include "fileIO.h"
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
        if(charFreq.size()!=256 && charFreq.count(buffer[0])==0){
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

void FileIO::encodeFile(string desFileName,map<char, string> charCode,map<char, long long> charFreq,int writeMode){
    ofstream fout;
    //如果是文件夹要写,就是追加写入
    if(writeMode==1){
        fout = ofstream(desFileName, ios::binary|ios::app);
    }
    else
        fout = ofstream(desFileName, ios::binary);
    //先将文件的头信息写好
    fileHead filehead;
    // 获取字符的种类,写头信息
    filehead.originBytes = file_size(sourceFileName);
    filehead.alphaVarity = charCode.size();
    fout.write((char *)&filehead, sizeof(filehead));
    //写字符的频度等等
    for(auto i:charFreq){
        alphaCode af(i);
        fout.write((char*)&af,sizeof(af));
    }
    //写文件的内容
    ifstream fin(sourceFileName,ios::binary);
    int bufferLength = 0;
    //每次只接收一个字符,于是会有2^8种字符
    char buffer = 0;
    while(!fin.eof()){
        fin.read(&buffer, sizeof(char));
        //获取此字符对应的哈夫曼编码
        string codeOfAlpha = ;
        for (int strIdx = 0; strIdx < charCode[buffer].length();strIdx++){
            buffer <<= 1;
            buffer |= (charCode[buffer][strIdx] == '1');
            bufferLength++;

            if(bufferLength==8){
                fout.write(&buffer, sizeof(char));
                buffer = 0;
            }
        }
            // //一旦字符串长度大于8,就将其转为一个char并写入文件
            // while (bitsToWrite.length() >= 8)
            // {
            //     char towrite = encode10to2(bitsToWrite.substr(0, 8));
            //     fout.write(&towrite, sizeof(char));
            //     bitsToWrite = bitsToWrite.substr(8, bitsToWrite.length() - 8);
            // }
    }
    //将最后不足8位的bit补全并写入
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

map<char, long long> FileIO::readFileHaffmanFreq(int alphaVariety){
    ifstream is(sourceFileName, ios::binary);
    map<char, long long> codeFreq;
    //定位在头信息后
    is.seekg(sizeof(fileHead));
    //将各个字符的频度获取
    for (int i = 0; i < alphaVariety;i++){
        alphaCode af;
        is.read((char *)&af, sizeof(af));
        codeFreq.insert(make_pair(af.alpha,af.freq));
    }
    is.close();
    return codeFreq;
}

void FileIO::decodeFile(fileHead filehead,map<char, long long> charFreq){
    ifstream is(sourceFileName, ios::binary);
    ofstream out(desFileName, ios::binary);
    //空文件直接return
    if(charFreq.size()==0){
        return;
    }
    //恢复哈夫曼树
    Haffman haffman = Haffman(charFreq);
    haffman.createHaffmanTree();
    Node root = haffman.nodeQueue.top();
    Node temp = root;
    //定位到存储文件的位置
    is.seekg(sizeof(filehead) + filehead.alphaVarity * sizeof(alphaCode),ios::beg);
    //开始读取
    char readBuf;
    long long writedBytes = 0;
    while(!is.eof()){
        is.read(&readBuf, sizeof(char));
        for (int i = 7; i >= 0;i--){
            if(readBuf&(1<<i))
                temp = *temp.right;
            else
                temp = *temp.left;
            if(haffman.isLeaf(&temp)){
                out.write(&temp.c, sizeof(char));
                temp = root;
                writedBytes++;
            }
            if(writedBytes>=filehead.originBytes){
                goto finish;
            }
        }
    }
    finish:out.close();
}