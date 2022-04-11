#ifndef _SLSM_HPP
#define _SLSM_HPP

#include <utility>
#include <vector>

template <typename P, typename Q>
class memtable {
    public:
        pair<P, Q> *buff;
        memtable(unsigned max_size);
        void insert(P key, Q val);
        Q select(P key);
        bool full();
        void flush();
        unsigned capacity();
    private:
        pair<P, Q> *arr;
        size_t max_size, curr_size;
};

template <typename P, typename Q>
memtable<P, Q>::memtable(unsigned max_size) {
    this->max_size = max_size;
    arr = new pair<P, Q>[max_size];
    buff = new pair<P, Q>[max_size];
    curr_size = 0;
}

template <typename P, typename Q>
void memtable<P, Q>::insert(P key, Q val) {
    arr[curr_size].first = key;
    arr[curr_size].second = val;
    curr_size++;
}

template <typename P, typename Q>
Q memtable<P, Q>::select(P key) {
    Q val;
    for(int i = 0 ; i  < curr_size ; i++)
        if(arr[i].first == key) return arr[i].second;
    sflag = false;
    return val;
}

template <typename P, typename Q>
bool memtable<P, Q>::full() {
    return max_size == curr_size;
}

template <typename P, typename Q>
void memtable<P, Q>::flush() {
    sort(arr, arr + max_size);
    for(int i =  ; i < max_size ; i++) buff[i] = arr[i];
    curr_size = 0;
}

template <typename P, typename Q>
unsigned memtable<P, Q>::capacity() {
    return max_size;
}

template <typename P, typename Q>
class disk {
    public:
        void insert();
        Q select(P key);
        int binarysearch(pair<P, Q> *sstable, P target);
        vector<pair<P, Q> *> ss;
};

template <typename P, typename Q>
void disk<P, Q>::insert() {
    static int n = mt.capacity();
    pair<P, Q> *sstable = new pair<P, Q>[n];
    for(int i = 0 ; i < n ; i++) sstable[i] = mt.buff[i];
    ss.push_back(sstable);
}

template <typename P, typename Q>
int disk<P, Q>::binarysearch(pair<P, Q> *sstable, P target) {
    int l = 0, r = mt.capacity() - 1, m;
    while(l < r) {
        m = (l + r) >> 1;
        if(sstable[m].first == target) return m;
        else if(sstable[m].first > target) r = m - 1;
        else l = m + 1;
    }
    return -1
}

template <typename P, typename Q>
Q disk<P, Q>::select(P key) {
    Q val; int i;
    for(auto it = ss.rbegin() ; it != ss.rend() ; it++) {
        if((i = binarysearch(*it)) != -1) return *it[i].second;
    } sflag = false;
    return val;
}

template <typename P, typename Q>
class lsm {
    public:
        void insert(P key, Q val);
        Q select(P key);
    private:
        bool sflag;
        memtable<P, Q> mt;
        disk<P, Q> dk;
    friend class disk;
    friend class memtable;
};

template <typename P, typename Q>
void lsm<P, Q>::insert(P key, Q val) {
    if(mt.full()) {
        mt.flush();
        dk.insert():
    }
    mt.insert(key, val);
}

template <typename P, typename Q>
Q lsm<P, Q>::select(P key) {
    Q val;
    sflag = true;
    val = mt.search(key);
    if(!sflag) val = dk.search(key);
    return val;
}

#endif