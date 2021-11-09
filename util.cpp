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
    //输入文件
    cout << "------------------------------------" << endl;
    cout << "please input the filename that you want to encode:" << endl << "(include the suffix)" << endl;
    string sourceFileName;
    cin >> sourceFileName;
    fstream testFile;
    vector<string> res;
    while(1){
        testFile.open(sourceFileName);
        if(!testFile){
            cout << "can not find this file,please input again:" << endl;
            cin >> sourceFileName;
        }else{
            testFile.close();
            res.push_back(sourceFileName);
            break;
        }
    }
    cout << "------------------------------------" << endl;
    //输出文件
    cout << "please input the output filename that you want:" << endl << "( Do not include the suffix, auto append '.hfm')" << endl;
    cout << "[! input '.' to use the origin filename]" << endl;
    string desFileName;
    cin >> desFileName;
    if(desFileName=="."){
        res.push_back(sourceFileName.substr(0, sourceFileName.find(".") + 1)+"hfm");
    }
    else{
        res.push_back(desFileName + ".hfm");
    }
    return res;
}

//todo
vector<string> getDecodeName(){
    //输入文件
    cout << "------------------------------------" << endl;
    cout << "please input the filename that you want to decode:" << endl << "( Do not include the suffix, auto append '.hfm')" << endl;
    string sourceFileName;
    cin >> sourceFileName;
    fstream testFile;
    vector<string> res;
    while(1){
        testFile.open(sourceFileName);
        if(!testFile){
            cout << "can not find this file,please input again:" << endl;
            cin >> sourceFileName;
        }else{
            testFile.close();
            res.push_back(sourceFileName);
            break;
        }
    }
    cout << "------------------------------------" << endl;
    //输出文件
    cout << "please input the output filename that you want:" << endl << "(include the suffix)" << endl;
    cout << "[! input '.' to use the origin filename]" << endl;
    string desFileName;
    cin >> desFileName;
    if(desFileName=="."){
        res.push_back(sourceFileName.substr(0, sourceFileName.find(".") + 1)+"hfm");
    }
    else{
        res.push_back(desFileName + ".hfm");
    }
    return res;
}