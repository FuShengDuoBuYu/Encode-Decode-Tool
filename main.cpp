#include "fileIO.h"

int main(int argc,char *argv[]){
    FileIO f = FileIO();
    f.getCharFreq("b.exe","test.exe");
    return 0;
}