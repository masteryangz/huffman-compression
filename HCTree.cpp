#include <string>
#include "HCTree.hpp"

void printHCTree(HCNode* n){
    cout<<n->count;
    cout<<" ";
    cout<<n->symbol;
    if(n->c0 != nullptr){
        cout<<"0 node: ";
        printHCTree(n->c0);
    }
    if(n->c1 != nullptr){
        cout<<"1 node: ";
        printHCTree(n->c1);
    }
}

void myClear(HCNode* n){
    if(n == nullptr){
        return;
    }
    n->p = nullptr;
    if(n->c0 != nullptr){
        myClear(n->c0);
        n->c0 = nullptr;
    }
    if(n->c1 != nullptr){
        myClear(n->c1);
        n->c1 = nullptr;
    }
    delete n;
}

HCTree::~HCTree(){
    myClear(root);
}

void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    int pqSize = 0;
    for(unsigned int i = 0; i < 256; i++){
        if(freqs[i]!=0){
            pqSize++;
            leaves[i] = new HCNode(freqs[i], i);
            pq.push(leaves[i]);
        }
    }
    
    // while(pq.top()->count==0){
    //     pq.pop();
    // }
    
    while(pqSize>1){
        HCNode* zeroNode = pq.top();
        pq.pop();
        pqSize--;
        HCNode* oneNode = pq.top();
        pq.pop();
        pqSize--;
        root = new HCNode((zeroNode->count)+(oneNode->count), zeroNode->symbol);
        root->c0 = zeroNode;
        root->c1 = oneNode;
        zeroNode->p = root;
        oneNode->p = root;
        pq.push(root);
        pqSize++;
    }
    if(pqSize==1){
        root = pq.top();
    }
    //printHCTree(root);
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    std::string code("");
    HCNode* curr = leaves[symbol];
    unsigned int codeLen = 0;
    while(curr->p!=nullptr){
        if(curr == curr->p->c0){
            code="0"+code;
            codeLen++;
        } else if(curr == curr->p->c1){
            code="1"+code;
            codeLen++;
        }
        curr = curr->p;
    }
    for(unsigned int i = 0; i < codeLen; i++){
        if(code[i]=='0'){
            out.write_bit(0);
        } else if(code[i]=='1'){
            out.write_bit(1);
        }
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const{
    if((root == nullptr)||(root->count == 0)){
        return 0;
    }
    HCNode* curr = root;
    while((curr->c0!=nullptr)&&(curr->c1!=nullptr)){
        int rbit = in.read_bit();
        if(rbit==0){
            curr = curr->c0;
        } else if(rbit==1){
            curr = curr->c1;
        } 
    }
    root->count--;
    return curr->symbol;
}
