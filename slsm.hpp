#ifndef _SLSM_HPP
#define _SLSM_HPP

#include <algorithm>
#include <thread>
#include <utility>
#include <vector>
#include "bf.hpp"
#include "skiplist.hpp"
#include "lsm.hpp"

using namespace std;

#define NO_OF_BLOCK 16

#define MAX_MSIZE 10

void *gbuff;

template <typename P, typename Q>
class slsm {
    public:
        slsm();
        slsm<P, Q>& insert(P key, Q val);
        Q select(P key);
        slsm<P, Q>& remove(P key);
    private:
        memtable<P, Q> mt;
        bf<int> *bftable;
        lsm<int, Q> *sblock;
};

template <typename P, typename Q>
slsm<P, Q>::slsm() {
    bftable = new bf<int>[16];
    sblock = new lsm<int, Q>[16];
}

template <typename P, typename Q>
slsm<P, Q>& slsm<P, Q>::remove(P key) {
    mt.remove(key);
    return *this;
}

template <typename P, typename Q>
Q slsm<P, Q>::select(P key) {
    pair<Q, bool> val;
    size_t hash_v = hash<P>()(key);
    int block_no = (hash_v / sizeof(int)) % 16;
    int trie_no = (hash_v % sizeof(int));
    val = mt.select(key);
    if(!val.second && !bftable[block_no].lookup(trie_no)) return val.first;
    if(!val.second) val = sblock[block_no].select(trie_no);
    return val.first;
}

template <typename P, typename Q>
slsm<P, Q>& slsm<P, Q>::insert(P key, Q val) {
    if(!mt.full()) mt.insert(key, val);
    else {
        mt.flush();
        for(int i = 0 ; i < MAX_MSIZE ; i++) {
            size_t hash_v = hash<P>()(((pair<P, Q> *)gbuff)[i].first);
            int block_no = (hash_v / sizeof(int)) % 16;
            int trie_no = (hash_v % sizeof(int));
            sblock[block_no].insert(trie_no, ((pair<P, Q> *)gbuff)[i].second);
            bftable[block_no].insert(trie_no);
        }
    }
    return *this;
}

#endif