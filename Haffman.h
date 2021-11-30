#ifndef HAFFMAN_H
#define HAFFMAN_H
#include <map>
#include <queue>
#include <iostream>
using namespace std;
//每一个带权的结点
class Node{
    public :
        char c;
        long long freq;
        Node *left;
        Node *right;
        Node(char c,long long freq){
            this->c = c;
            this->freq = freq;
            left = nullptr;
            right = nullptr;
        }
        Node(char c,long long freq,Node* leftChild,Node* rightChild){
            this->c = c;
            this->freq = freq;
            left = leftChild;
            right = rightChild;
        }
        //重载运算符,原始的底层是最大堆,我们要用最小堆,保证其队列是按照权值从小到大
        bool operator<(const Node &node) const{
            return freq > node.freq;
        }
};

class Haffman{
    public :
        //获取哈夫曼编码
        map<char, string> createHaffmanCode();
        //生成一个哈夫曼树
        void createHaffmanTree();
        //用于生成哈夫曼树的优先队列
        priority_queue<Node> nodeQueue;
        //判断是否是叶子结点
        static bool isLeaf(Node* node){
            return node->left == nullptr && node->right == nullptr;
        }
        //构造方法
        Haffman(map<char, long long> charFreq){
            //通过map来构建一个以频率为权值的优先队列
            for(auto i:charFreq){
                // cout << i.second << endl;
                Node node(i.first, i.second);
                nodeQueue.push(node);
            }
        }

        ~Haffman() {
            Node node = nodeQueue.top();
            deleteNode(node.left);
            deleteNode(node.right);
        }
    private:
        
        
        //递归删除结点
        void deleteNode(Node* node);
        
        //递归生成哈夫曼编码的子函数
        void subCreateHaffmanCode(Node* root,string prefix,map<char, string>& charCode);
};


#endif