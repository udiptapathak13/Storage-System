#ifndef _SKIP_LIST_HPP
#define _SKIP_LIST_HPP

#define SKIP_MAX_LVL 4

#include <iostream>

using namespace std;

template <typename P, typename Q>
class skipnode {
    public:
        P key; Q *val;
        skipnode(P key, Q val);
        skipnode(P key, Q val, int n);
        skipnode **nextarr;
        unsigned lvl;
        void print();
};

template <typename P, typename Q>
skipnode<P, Q>::skipnode(P key, Q val) {
    this->key = key;
    this->val = new Q;
    *(this->val) = val;
    lvl = 1 + (rand() % SKIP_MAX_LVL);
    nextarr = new skipnode<P, Q>*[lvl];
    for(int i = 0 ; i < lvl ; i++)
        nextarr[i] = NULL;
}

template <typename P, typename Q>
skipnode<P, Q>::skipnode(P key, Q val, int n) {
    this->key = key;
    this->val = new Q;
    *(this->val) = val;
    lvl = n;
    nextarr = new skipnode<P, Q>*[lvl];
    for(int i = 0 ; i < lvl ; i++)
        nextarr[i] = NULL;
}

template <typename P, typename Q>
void skipnode<P, Q>::print() {
    cout<<"("<<key<<":"<<*val<<";";
    for(int i = 0 ; i < lvl ; i++) if(nextarr[i]) cout<<nextarr[i]->key<<",";
    cout<<"\x8)";
}

template <typename P, typename Q>
class skiplist {
    public:
        skiplist();
        skipnode<P, Q> *root;
        skiplist<P, Q>& insert(P key, Q val);
        skiplist<P, Q>& remove(P key);
        Q select(P key);
        void print();
    public:
        unsigned sz;
};

template <typename P, typename Q>
skiplist<P, Q>::skiplist() {
    sz = 0;
    root = NULL;
}

template <typename P, typename Q>
skiplist<P, Q>& skiplist<P, Q>::insert(P key, Q val) {
    if(!sz) root = new skipnode<P, Q>(key, val, SKIP_MAX_LVL);
    else if(key < root->key) {
        skipnode<P, Q> *newnode = new skipnode<P, Q>(key, val, SKIP_MAX_LVL);
        for(int i = 0 ; i < SKIP_MAX_LVL ; i++) newnode->nextarr[i] = root;
        root = newnode;
    } else {
        skipnode<P, Q> *newnode = new skipnode<P, Q>(key, val), *temp = root;
        int newlvl = newnode->lvl; 
        int i = SKIP_MAX_LVL - 1;
        while(i >= 0) {
            while(temp->nextarr[i] && temp->nextarr[i]->key < key) temp = temp->nextarr[i];
            if(i < newlvl) {
                newnode->nextarr[i] = temp->nextarr[i];
                temp->nextarr[i] = newnode;
            }
            i--;
        }
    }
    sz++;
    return *this;
}

template <typename P, typename Q>
Q skiplist<P, Q>::select(P key) {
    skipnode<P, Q> *temp = root;
    int i = SKIP_MAX_LVL;
    Q val;
    while(i >= 0) {
        while(temp->nextarr[i] && temp->nextarr[i]->key <= key) temp = temp->nextarr[i];
        if(temp->key == key) return *(temp->val);
        i--;
    }
    return val;
}

template <typename P, typename Q>
skiplist<P, Q>& skiplist<P, Q>::remove(P key) {
    if(!sz) return *this;
    if(root->key == key && sz == 1) {
        delete root;
        root = NULL;
        sz--;
        return *this;
    }
    if(root->key == key) {
        skipnode<P, Q> *temp = root->nextarr[0];
        root->key = root->nextarr[0]->key;
        *(root->val) = *(root->nextarr[0]->val);
        int tlvl = root->nextarr[0]->lvl;
        for(int i = 0 ; i < tlvl ; i++)
            root->nextarr[i] = root->nextarr[0]->nextarr[i];
        delete temp;
        sz--;
        return *this;
    }
    skipnode<P, Q> *temp = root, *ptr;
    int i = SKIP_MAX_LVL;
    while(i >= 0) {
        while(temp->nextarr[i] && temp->nextarr[i]->key < key) temp = temp->nextarr[i];
        if(temp->nextarr[i] && temp->nextarr[i]->key == key) 
            if(i) temp->nextarr[i] = temp->nextarr[i]->nextarr[i];
            else {
                ptr = temp->nextarr[i];
                temp->nextarr[i] = temp->nextarr[i]->nextarr[i];
                delete ptr;
            }
        i--;
    }
    sz--;
    return *this;
}

template <typename P, typename Q>
void skiplist<P, Q>::print() {
    skipnode<P, Q> *temp = root;
    while(temp) {
        temp->print();
        temp = temp->nextarr[0];
    } cout<<"\xa";
}

#endif