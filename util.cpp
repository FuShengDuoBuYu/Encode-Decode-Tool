#include "util.h"
char encode10to2(string s){
    bitset<8> bit(s);
    char res = bit.to_ulong();
    return res;
}

int getEncodeOrDecode(){
    cout << "======================Welcome To Use Haffman Encode & Decode Tool========================" << endl 
         << "Encode: input '1'" << endl << "Decode: input '2' :" << endl;
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