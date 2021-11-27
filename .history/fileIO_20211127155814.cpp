#include "fileIO.h"
map<char, long long> FileIO::getCharFreq(){
    //用二进制流传输
    ifstream fin(sourceFileName,ios::binary);
    //用一个map匹配字符和其出现的频度
    map<char, long long> charFreq;
    long long charFreqArray[256] = {0L};
    while(!fin.eof()){
        //每次只接收一个字符,于是会有2^8种字符
        char buffer;
        fin.read(&buffer, sizeof(char));
        //如果map里没有这个字符,就加入这个字符并将其频度设为1
        if(charFreq.size()!=256 && charFreq.count(buffer)==0){
            charFreq.insert(map<char, long long>::value_type(buffer, 1L));
        }
        //如果map中有这个字符,就将其频度++
        else{
            charFreq[buffer]++;
        }
    }
    // map<char, long long>::reverse_iterator   iter;
    //  for(iter = charFreq.rbegin(); iter != charFreq.rend(); iter++){
    //       cout<<(int)iter->first<<endl;
    //  }
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
    unsigned char bufferbit = 0;
    char buffer;
    char bufferArray[1024];
    int bufferArrayIndex = 0;
    while(!fin.eof()){
        fin.read(&buffer, sizeof(char));
        string codeOfAlpha = charCode[buffer];
        for (int strIdx = 0; strIdx < codeOfAlpha.length();strIdx++){
            bufferbit <<= 1;
            bufferbit |= (codeOfAlpha[strIdx] == '1');
            bufferLength++;
            //当有一个字符的时候就放进待写数组里
            if(bufferLength==8){
                bufferArray[bufferArrayIndex] = bufferbit;
                bufferArrayIndex++;
                bufferbit = 0;
                bufferLength = 0;
            }
            //如果buffer数组满了,就写入文件
            if(bufferArrayIndex==1024){
                fout.write(bufferArray, sizeof(char) * 1024);
                bufferArrayIndex = 0;
            }
        }
    }
    //将最后不足8位的bit补全并写入
    if(bufferLength!=0){
        while(bufferLength!=8){
            bufferbit <<= 1;
            bufferLength++;
        }
        bufferArray[bufferArrayIndex] = bufferbit;
        bufferArrayIndex++;
        
    }
    //写数组剩下的东西
    if(bufferArrayIndex!=0){
        fout.write(bufferArray, bufferArrayIndex * sizeof(char));
    }
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