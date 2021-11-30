# 压缩解压工具 -- 开发文档



## 第一部分 项目文件结构说明

### 文件说明

本PJ共有7个核心代码文件,分别如下所示.

具体函数的声明以及其作用在源代码中已注释,此处不再赘述.只对文件的功能和结构简要概述

- **main.cpp**
  - 文件的入口,用来组织各个文件的方法,以实现文件(夹)的压缩与解压缩
- **fileIO.cpp & fileIO.h**
  - 含有filehead和alphacode的struct,用来记录文件头信息和字符频度等
  - 含有fileIO类,用来对文件(夹)进行IO操作,并且获取文件中的基本信息.
    - 例如
      - 压缩单个文件并输出
      - 获取单个文件的头信息
      - 获取单个文件的字符出现频度
      - ...
- **Haffman.cpp & Haffman.h**
  - 主要用于接收fileIO获取的文件初步信息后进行哈夫曼树以及哈夫曼编码的构造
  - 含有Node类,表示每一个字符为带权的结点
  - 含有Haffman类,用于生成哈夫曼树和哈夫曼编码
- **util.cpp & util.h**
  - 里面包含一些用到的工具函数
    - 例如
      - 将字符串转为char的函数
      - 与用户交互的函数
      - 获取压缩完各个文件大小的函数
      - ...

---



## 第二部分 项目设计,实现的具体思路

### 设计思路

将本PJ视为两个部分,第一部分为单个文件的压缩与解压,第二个部分为文件夹的压缩和解压

考虑到文件夹总会包含子文件(夹),因此可以在设计实现第二个部分时复用并合理优化第一部分的代码

通过在程序的开始时与用户交互获取用户需求,如果需要解压/压缩单一文件,进行第一部分;如果需要压缩/解压文件夹,进行第二部分

---

### 实现思路

**第一部分(单个文件压缩/解压)**

- **压缩**

  - 首先读取文件中各个字符的频度,然后利用这些频度构建出该文件的haffman编码,并将**这些haffman树/编码的头信息写入到目标文件中**

  - 最后再扫描一遍文件,将对应字符的haffman编码写入目标文件中

- **解压**
  - 首先读取压缩文件的开头的头信息部分,然后利用头信息恢复该文件的haffman树结构
  - 最后依次读取压缩文件的信息,通过回复的haffman树来将解压缩后的信息写入目标文件

---

**第二部分(文件夹压缩/解压)**

- **压缩**
  - 首先当发现用户输入的是文件夹时,先在目标文件头部输入该文件夹下所有子文件(夹)的信息(如文件名,路径等).
  - 然后复用单个文件的压缩,依次将各个子文件的压缩后的文件写入目标文件
  - 最后在目标文件的最后记录各个压缩后的子文件的大小,以记录各个部分是属于哪个子文件
- **解压**
  - 首先读取头信息中的文件目录信息,恢复原文件夹目录结构
  - 然后读取文件末尾的各个子文件的压缩后的大小,并将各个子文件分离开
  - 最后复用单个文件的解压缩,将各个解压缩后的文件写入原文件夹目录结构中

---



## 第三部分 性能测试结果

部分文件压缩测试如下表所示,硬件配置:

CPU: Intel i5-8265U 最高主频 1.6Ghz

内存: 8G

| 文件名                                | 压缩后大小/bytes | 原大小/bytes | 其他压缩工具(haozip)/bytes | 压缩率 | 解压时间/ms | 压缩时间/ms |
| ------------------------------------- | ---------------- | ------------ | -------------------------- | ------ | ----------- | ----------- |
| Test Cases\test1 - single file\1.txt  | 1,143,604        | 2,018,251    | 738,657                    | 56.7%  | 131         | 182         |
| Test Cases\test1 - single file\35.avi | 33,868           | 34,180       | 26,244                     | 99%    | 9           | 10          |
| Test Cases\test1 - single file\7.pdf  | 106,083          | 130,002      | 93,264                     | 81.6%  | 19          | 25          |
|                                       |                  |              |                            |        |             |             |
| Test Cases\test2 - folder\1           | 1,588,361        | 2,521,912    | 606,153                    | 62.9%  | 248         | 326         |
| Test Cases\test2 - folder\2           | 6,347,988        | 9,797,013    | 2,308,914                  | 64.5%  | 895         | 1195        |
| Test Cases\test2 - folder\3           | 1,705,052        | 2,587,851    | 584,662                    | 67.6%  | 242         | 343         |
|                                       |                  |              |                            |        |             |             |
| Test Cases\test4 - large file\1.jpg   | 20,694,200       | 20,748,246   | 20,688,868                 | 99.7%  | 2688        | 2658        |
| Test Cases\test4 - large file\2.csv   | 74,798,252       | 116,596,836  | 31,005,769                 | 64.1%  | 9084        | 10812       |
| Test Cases\test4 - large file\3.csv   | 74,230,796       | 116,523,008  | 14,364,109                 | 63.7%  | 7117        | 10626       |
|                                       |                  |              |                            |        |             |             |
| Test Cases                            | 186,276,005      | 277,987,063  | 74,609,210                 | 67%    | 22082       | 27232       |

---



## 第四部分 项目重难点

### 一. 二进制文件写入

**问题:**

​	存储某个字符的哈夫曼编码时,刚一开始使用的是包含'0'和'1'的字符串存储,但是这样子会造成大量字符串的拼接,从而带来极大的时间消耗

**解决方案:**

​	利用字符的位移运算,从而只进行字符串的某个字符的比对,不再进行字符串的拼接,从而提高的时间效率.

​	代码示例如下:

```c++
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
```

---

### 二 . 复用单文件压缩时难以获取压缩后文件的大小

**问题:**

​	在复用单个文件压缩来压缩文件夹时,需要知道每个子文件压缩后的大小才能对文件夹压缩后的文件解压时正确分离.但是只有在压缩完子文件后才能知道压缩后子文件的大小.

**解决方案:**

​	压缩子文件的函数返回压缩后的大小,将所有子文件压缩后的大小写在目标文件的末尾,解压文件夹时先读取末尾的文件大小值

```c++
	// 写各个二进制文件
    vector<long long> afterSizes;
    for (int i = 0; i < fileName.size(); i++){
        int aftersize = encodeSingleFile(headPath + fileName[i], desFilename, 1);
        afterSizes.push_back(aftersize);
    }
    // 写压缩后的大小
    ofstream fout(desFilename,ios::app);
    fout << "\n";
    for (int i = 0; i < afterSizes.size();i++){
        fout << " " << afterSizes[i] ;
    }
    
    //.................
    
    //获取压缩后的大小
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
```



### 其他

1.写代码时有时候在文件IO时想尽可能开大一点数组,但是程序莫名崩溃,最终确定应该是函数里开数组是在栈上开数组,因此不能大于1024*1024

2.二进制文件的解压也同样用到了位移运算,思路一致,此处不再赘述

3.限于篇幅,其他不再详述

## 第五部分 优化技巧

### 一 . 利用数组存储字符频度

**优化前:**

利用每一个字符来构建哈夫曼编码,于是则有256个叶子,于是刚开始时使用的是C++的map来存储每个字符的频度.

我们知道,map的查找效率是O(lgn),但是对于每个字符都需要在里面查找并且找到后频度++的时间开销还是很大的

**优化后:**

由于一个char的大小在-128到127,而unsigned char则在0-255之间,于是可以构建一个大小为256的数组.

我们知道,数组的查找效率是O(1),因此在扫描文件获取字符频度的时候可以节省许多时间

```c++
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
```

**实测压缩时可以解决约一半的时间**

---

### 二 . 压缩或解压文件时减少IO次数

**优化前:**

​	如果我们每次发现需要写一个char时,就调用write函数写入文件,那么当文件很大时,我们有大量的时间就花费在文件的打开和关闭上了.

**优化后:**

​	创建一个1M大小的缓冲数组,每次将要写的字符存在数组中,如果数组满了,那么一次将整个1M数组的数据写入目标文件,减少文件IO次数

```c++
	//以解压为例
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
```

---

### 三 . 在文件头使用struct记录信息

**优化前:**

​	在写文件头信息时,如果用各种自定义的记号,比如两个'*'之间代表某某数据,显得会很杂乱无章

**优化后:**

​	在头信息写入时直接以struct的形式写入,这样读取头信息时也只需要完整的读取一个struct就可以完全获取头信息

```c++
	// 每个文件的头部,包含文件的基信息
    struct fileHead{
        //字符种类
        int alphaVarity;
        //源文件的字节数
        long long originBytes;
    };
    //字母及其频度
    struct alphaCode{
        char alpha;
        //频度
        long long freq;
        alphaCode(){}
        alphaCode(const pair<char ,long long>& x){
            this->alpha = x.first;
            this->freq = x.second;
        }
    };
	
	//........................
	
	
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
```

---

### 其他

还有其他一些小细节的优化,例如**用filesize获取文件大小后就避免了eof的判断问题**等等,此处限于篇幅,便不再赘述

---



## Bonus 思考标准压缩和哈夫曼压缩之间的差异

通过上面的表格和实践,可以知道标准压缩在某些文件上和哈夫曼压缩差别不大,有些则是胜过许多.

标准工具胜过哈夫曼编码的原因:

- 不仅仅使用哈夫曼编码,可能会使用其他更优压缩编码格式
- 对于计算机的资源调度和IO管理比我们自己写的更加规范合理,因此时间更快
- 采用更加合理高效的算法
- ....



标准工具和哈夫曼编码差别不大的原因:

- 某些工具对某些文件就是采用了哈夫曼编码
- 某些源文件已经进行过了压缩,因此不再能产生很高的压缩率
- ...
