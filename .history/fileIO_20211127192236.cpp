#include "fileIO.h"
map<char, long long> FileIO::getCharFreq(){
    //用二进制流传输
    ifstream fin(sourceFileName,ios::binary);
    //用一个map匹配字符和其出现的频度
    map<char, long long> charFreq;
    long long charFreqArray[256] = {0L};
    char buffer;
    while(!fin.eof()){
        //每次只接收一个字符,于是会有2^8种字符
        fin.read(&buffer, sizeof(char));
        //对应数组下标位置加一
        charFreqArray[(int)buffer + 128]++;
    }
    //将有数据的字符写进map
    for (int i = 0; i < 256;i++){
        if(charFreqArray[i]!=0){
            charFreq.insert(map<char, long long>::value_type((char)i-128, charFreqArray[i]));
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
    //将哈夫曼码放到一个结构数组里
    struct CharInfo{
		CharInfo() : code("") { }
		string code;
	}charInfoArray[256];
    //字母及其哈夫曼码
    map<char, string>::reverse_iterator   iter;
    for(iter = charCode.rbegin(); iter != charCode.rend(); iter++){
        charInfoArray[(int)iter->first + 128].code = iter->second;
    }
    //写文件的内容
    ifstream fin(sourceFileName,ios::binary);
    int bufferLength = 0;
    //每次只接收一个字符,于是会有2^8种字符
    unsigned char bufferbit = 0;
    char buffer;
    char bufferArray[1024*1024];
    int bufferArrayIndex = 0;
    while(!fin.eof()){
        fin.read(&buffer, sizeof(char));
        for (int strIdx = 0; strIdx < charInfoArray[(int)buffer+128].code.length();strIdx++){
            bufferbit <<= 1;
            bufferbit |= (charInfoArray[(int)buffer+128].code[strIdx] == '1');
            bufferLength++;
            //当有一个字符的时候就放进待写数组里
            if(bufferLength==8){
                bufferArray[bufferArrayIndex] = bufferbit;
                bufferArrayIndex++;
                bufferbit = 0;
                bufferLength = 0;
            }
            //如果buffer数组满了,就写入文件
            if(bufferArrayIndex==1024 * 1024){
                fout.write(bufferArray, sizeof(char) * 1024 *1024);
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
    cout << "decode" << endl;
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
    char writeBufferArray[1024 * 1024];
    int writeBufferArrayIndex = 0;
    
    while(!is.eof()){
        is.read(&readBuf, sizeof(char));
        for (int i = 7; i >= 0;i--){
            if(readBuf&(1<<i))
                temp = *temp.right;
            else
                temp = *temp.left;
            if(haffman.isLeaf(&temp)){
                // out.write(&temp.c, sizeof(char));
                //该字符放到缓存数组里
                writeBufferArray[writeBufferArrayIndex] = temp.c;
                //缓存指针加一
                writeBufferArrayIndex++;
                temp = root;
                writedBytes++;
            }
            //缓存数组满,写入文件
            if(writeBufferArrayIndex==1024*1024){
                out.write(writeBufferArray, 1024 * 1024 * sizeof(char));
                writeBufferArrayIndex = 0;
            }
            if(writedBytes>=filehead.originBytes){
                goto finish;
            }
        }
    }
    //将残留的数据写进去
    finish: ;
        out.write(writeBufferArray, writeBufferArrayIndex * sizeof(char));
        out.close();
    
    
    
    
}