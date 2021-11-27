#include <iostream>
#include <string>
#include <vector>
int main(){
    vector<string> res;
    //输入文件(夹)
    cout << "------------------------------------" << endl;
    cout << "please input the filename that you want to encode:" << endl << "(include the suffix)" << endl;
    
    //输入的文件(夹)目录
    while(1){
        string sourceFileName;
        cin >> sourceFileName;
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
            cin >> sourceFileName;
        }
        //文件
        else{
            res.push_back("1");
            res.push_back(sourceFileName);
            break;
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