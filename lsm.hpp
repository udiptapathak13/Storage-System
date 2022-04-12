#ifndef _LSM_HPP
#define _LSM_HPP

#include <algorithm>
#include <utility>
#include <vector>

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
        int binarysearch(pair<P, Q*> *sstable, P target);
        vector<pair<P, Q*> *> ss;
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
    pair<P, Q*> *sstable = new pair<P, Q*>[n];
    for(int i = 0 ; i < n ; i++) {sstable[i] = ((pair<P, Q*> *) buff)[i]; if(sstable[i].second) cout<<"("<<sstable[i].first<<","<<*sstable[i].second<<")"; else cout<<"fuck ";} cout<<endl;
    ss.push_back(sstable);
}

template <typename P, typename Q>
int disk<P, Q>::binarysearch(pair<P, Q*> *sstable, P target) {
    int l = 0, r = msize - 1, m;
    while(l <= r) {
        m = ((l + r) >> 1) + 1;
        if(sstable[r].first == target) return sstable[r].second ? r : -1;
        else if(sstable[m].first > target) r = m - 1;
        else if(sstable[m].first < target) l = m + 1;
        else l = m;
    }
    return -1;
}

template <typename P, typename Q>
pair<Q, bool> disk<P, Q>::select(P key) {
    Q val; int i;
    for(auto it = ss.rbegin() ; it != ss.rend() ; it++) {
        if((i = binarysearch(*it, key)) != -1) return {*(*it)[i].second, true};
    }
    return {val, false};
}

template <typename P, typename Q>
class lsm {
    public:
        lsm();
        void insert(P key, Q val);
        void remove(P key);
        Q select(P key);
    private:
        memtable<P, Q> mt;
        disk<P, Q> dk;
};

template <typename P, typename Q>
lsm<P, Q>::lsm() {
    
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
Q lsm<P, Q>::select(P key) {
    pair<Q, bool> val;
    val = mt.select(key);
    if(!val.second) val = dk.select(key);
    return val.first;
}

#endif