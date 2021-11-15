#include "Haffman.h"

map<char, string> Haffman::createHaffmanCode(){ 
    string haffmanCode;
    map<char, string> charCode;
    createHaffmanTree();
    Node root = nodeQueue.top();
    subCreateHaffmanCode(&root,haffmanCode,charCode); 
    return charCode;
}

void Haffman::deleteNode(Node* node){
    if(node!=nullptr){
        deleteNode(node->left);
        deleteNode(node->right);
        delete node;
    }
    else{
        return;
    }
}

void Haffman::createHaffmanTree(){
    //当只剩下一个结点的时候才是最后的根结点
    while(nodeQueue.size()!=1){
        //把第一个出队并设置为一个父节点的左孩子
        Node *leftChild = new Node(nodeQueue.top());
        nodeQueue.pop();
        //把第二个出队并设置为一个父节点的左孩子
        Node *rightChild = new Node(nodeQueue.top());
        nodeQueue.pop();
        //设置临时结点,T代表Temp
        Node node('T', leftChild->freq + rightChild->freq,leftChild,rightChild);
        //将临时结点放到优先队列里
        nodeQueue.push(node);
    }
}

void Haffman::subCreateHaffmanCode(Node* root,string prefix,map<char, string>& charCode){
    //用于给右子树备份,从当前位置开始再往后加
    string temp = prefix;
    //左子树加0
    if (root->left != nullptr){
        prefix += '0';
        //如果此时其左孩子是叶子,就将其记录写入map
        if(isLeaf(root->left)){
            charCode[root->left->c] = prefix;
        }
        else{
            subCreateHaffmanCode(root->left,prefix,charCode);
        }
    }
    //右子树加1
    if(root->right!=nullptr){
        prefix = temp;
        prefix += '1';
        //如果此时其右孩子是叶子,就将其记录写入map
        if(isLeaf(root->right)){
            charCode[root->right->c] = prefix;
        }
        else{
            subCreateHaffmanCode(root->right,prefix,charCode);
        }
    }
}