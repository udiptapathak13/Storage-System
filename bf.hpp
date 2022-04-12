#ifndef _BF_HPP
#define _BF_HPP

#include <functional>
#include <bitset>

#define MAX_BIT_SIZE 32

using namespace std;

template <typename P>
class bf {
    public:
        bf();
        bf& insert(P key);
        bool lookup(P key);
    private:
        unsigned bit_count;
        bitset<MAX_BIT_SIZE> bset;
};

template <typename P>
bf<P>::bf() {
    bit_count = MAX_BIT_SIZE;
}

template <typename P>
bf<P>& bf<P>::insert(P key) {
    int arr[3];
    arr[0] = hash<P>()(key) % MAX_BIT_SIZE;
    for(int i = 1 ; i < 3 ; i++)
         arr[i] = hash<string>()(to_string(arr[i - 1])) % MAX_BIT_SIZE;
    for(int i = 0 ; i < 3 ; i++)
        bset.set(arr[i]);
    return *this;
}

template <typename P>
bool bf<P>::lookup(P key) {
    int arr[3];
    arr[0] = hash<P>()(key) % MAX_BIT_SIZE;
    for(int i = 1 ; i < 3 ; i++)
        arr[i] = hash<string>()(to_string(arr[i - 1])) % MAX_BIT_SIZE;
    for(int i = 0 ; i < 3 ; i++)
        if(!bset[arr[i]]) return false;
    return true;
}

#endif