#include "util.h"
char encode10to2(string s){
    bitset<8> bit(s);
    char res = bit.to_ulong();
    return res;
}

int getEncodeOrDecode(){
    cout << "======================Welcome To Use Haffman Encode & Decode Tool========================" << endl 
         << endl << "Encode: input '1'" << endl << "Decode: input '2' :" << endl;
    char mode;
    cin >> mode;
    while(1){
        if(mode==49){
            return 1;
        }
        else if(mode==50){
            return 2;
        }
        else{
            cout << "your input is invalid,input it again:" << endl;
            cin >> mode;
        }
    }
}

int encode2to10(string s){
    bitset<256> bit(s);
    int res = bit.to_ulong();
    return res;
}

string encode10to2(int length, int value){
    bitset<256> bit(value);
    string temp = bit.to_string();
    return temp.substr(temp.length()-length,length);
}

vector<string> getEncodeName(){
    vector<string> res;
    //输入文件(夹)
    cout << "------------------------------------" << endl;
    cout << "please input the filename that you want to encode:" << endl << "(include the suffix)" << endl;
    string sourceFileName;
    getline(cin, sourceFileName);
    //输入的文件(夹)目录
    while(1){
        char a[100];
        path str(sourceFileName);
        directory_entry entry(str);
        //文件夹
        if(entry.status().type() == file_type::directory){
            res.push_back("0");
            res.push_back(sourceFileName);
            break;
        }
        //未找到文件
        else if(entry.status().type() == file_type::not_found){
            cout << "not found file or dir. input again:" << endl;
        }
        //文件
        else{
            res.push_back("1");
            res.push_back(sourceFileName);
        }
    }
    cout << "------------------------------------" << endl;
    //输出文件(夹)
    cout << "please input the output filename that you want:" << endl << "(include the suffix)" << endl;
    string desFileName;
    cin >> desFileName;
    res.push_back(desFileName);
    return res;
}

vector<string> getDecodeName(){
    //输入文件
    cout << "------------------------------------" << endl;
    cout << "please input the filename that you want to decode:" << endl << "(include the suffix)" << endl;
    string sourceFileName;
    getline(cin, sourceFileName);
    cout << sourceFileName << endl;
    fstream testFile;
    vector<string> res;
    while(1){
        testFile.open(sourceFileName);
        if(!testFile){
            cout << "can not find this file,please input again:" << endl;
            cin >> sourceFileName;
        }else{
            //判断是文件夹还是文件
            string firstLine;
            getline(testFile, firstLine);
            if(firstLine=="0"){
                //文件夹
                res.push_back("0");
            }
            else{
                if(atoi(firstLine.c_str())==0){
                    //文件
                    res.push_back("1");
                }
                else{
                    //文件夹
                    res.push_back("0");
                }
            }
            testFile.close();
            res.push_back(sourceFileName);
            break;
        }
    }
    cout << "------------------------------------" << endl;
    //输出文件
    cout << "please input the output file/dir name that you want:" << endl << "(include the suffix)" << endl;
    string desFileName;
    cin >> desFileName;
    res.push_back(desFileName);
    
    return res;
}

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