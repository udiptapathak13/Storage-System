#ifndef _LSM_HPP
#define _LSM_HPP

#include <algorithm>
#include <thread>
#include <utility>
#include <vector>
#include "bf.hpp"

using namespace std;

#define MAX_MSIZE 10

void *buff;

template <typename P, typename Q>
class memtable {
    public:
        memtable();
        void insert(P key, Q val);
        void remove(P key);
        pair<Q, bool> select(P key);
        bool full();
        void flush();
        unsigned capacity();
    private:
        pair<P, Q*> *arr;
        size_t max_size, curr_size;
};

template <typename P, typename Q>
memtable<P, Q>::memtable() {
    this->max_size = MAX_MSIZE;
    arr = new pair<P, Q*>[max_size];
    buff = new pair<P, Q*>[max_size];
    curr_size = 0;
}

template <typename P, typename Q>
void memtable<P, Q>::insert(P key, Q val) {
    arr[curr_size].first = key;
    Q *valptr = new Q;
    *valptr = val;
    arr[curr_size].second = valptr;
    curr_size++;
}

template <typename P, typename Q>
void memtable<P, Q>::remove(P key) {
    arr[curr_size].first = key;
    arr[curr_size].second = NULL;
    curr_size++;
}

template <typename P, typename Q>
pair<Q, bool> memtable<P, Q>::select(P key) {
    Q val;
    for(int i = curr_size - 1; i >= 0 ; i--)
        if(arr[i].second && arr[i].first == key) return {*arr[i].second, true};
        else if(arr[i].first == key) break;
    return {val, false};
}

template <typename P, typename Q>
bool memtable<P, Q>::full() {
    return max_size == curr_size;
}

template <typename P, typename Q>
void memtable<P, Q>::flush() {
    stable_sort(arr, arr + max_size, [](pair<P, Q*> p, pair<P, Q*> q) {return p.first < q.first;});
    for(int i = 0 ; i < max_size ; i++) ((pair<P, Q*> *) buff)[i] = arr[i];
    curr_size = 0;
}

template <typename P, typename Q>
unsigned memtable<P, Q>::capacity() {
    return max_size;
}

template <typename P, typename Q>
class disk {
    public:
        disk();
        void insert();
        pair<Q, bool> select(P key);
        vector<vector<pair<P, Q*>>> ss;
    private:
        unsigned msize;
};

template <typename P, typename Q>
disk<P, Q>::disk() {
    msize = MAX_MSIZE;
}

template <typename P, typename Q>
void disk<P, Q>::insert() {
    static int n = msize;
    vector<pair<P, Q*>> sstable;
    bf<P> b;
    for(int i = n - 1 ; i >= 0 ; i--) 
        if(!b.lookup(((pair<P, Q*> *) buff)[i].first)) {
            b.insert(((pair<P, Q*> *) buff)[i].first);
            sstable.push_back(((pair<P, Q*> *) buff)[i]);
        } else {
            if(!binary_search(sstable.begin(), sstable.end(), ((pair<P, Q*> *) buff)[i], 
                [](pair<P, Q*> p, pair<P, Q*> q) {return p.first > q.first;}))
                sstable.push_back(((pair<P, Q*> *) buff)[i]);
        }
    reverse(sstable.begin(), sstable.end());
    ss.push_back(sstable);
}

template <typename P, typename Q>
pair<Q, bool> disk<P, Q>::select(P key) {
    Q val; int l, r, m;
    for(auto it = ss.rbegin() ; it != ss.rend() ; it++) {
        l = 0; r = (*it).size() - 1;
        while(l <= r) {
            m = (l + r) >> 1;
            if((*it)[m].first == key) {
                if((*it)[m].second) return {*(*it)[m].second, true};
                else return {val, false};
            }
            if((*it)[m].first > key) r = m - 1;
            else l = m + 1;
        }
    }
    return {val, false};
}

template <typename P, typename Q>
class lsm {
    public:
        lsm();
        void insert(P key, Q val);
        void remove(P key);
        pair<Q, bool> select(P key);
    private:
        void operator() ();
        memtable<P, Q> mt;
        disk<P, Q> dk;
};

template <typename P, typename Q>
void lsm<P, Q>::operator() () {
    int n = dk.ss.size() - 1;
    if(n < 2) return;
    int len1 = dk.ss[n].size(), len2 = dk.ss[n - 1].size();
    vector<pair<P, Q>> v;
    bf<P> b;
    for(int i = len2 ; i >= 0 ; i--) {
        if(!b.lookup(dk.ss[n - 1][i].first)) {
            b.insert(dk.ss[n - 1][i].first);
            v.push_back(dk.ss[n - 1][i]);
        } else if(dk.binarysearch(v, dk.ss[n - 1][i]) == -1)
                v.push_back(dk.ss[n - 1][i]);
    }
    for(int i = len2 ; i >= 0 ; i--) {
        if(!b.lookup(dk.ss[n][i].first)) {
            b.insert(dk.ss[n][i].first);
            v.push_back(dk.ss[n][i]);
        } else if(dk.binarysearch(v, dk.ss[n][i]) == -1)
                v.push_back(dk.ss[n][i]);
    } dk.ss.erase(dk.ss.begin() + n);
    dk.ss[n - 1] = v;
}  

template <typename P, typename Q>
lsm<P, Q>::lsm() {
    thread compact(lsm());
}

template <typename P, typename Q>
void lsm<P, Q>::insert(P key, Q val) {
    if(mt.full()) {
        mt.flush();
        dk.insert();
    }
    mt.insert(key, val);
}

template <typename P, typename Q>
void lsm<P, Q>::remove(P key) {
    mt.remove(key);
}

template <typename P, typename Q>
pair<Q, bool> lsm<P, Q>::select(P key) {
    pair<Q, bool> val;
    val = mt.select(key);
    if(!val.second) val = dk.select(key);
    return val;
}

#endif