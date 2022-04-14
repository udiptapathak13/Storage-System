#include <iostream>
#include <algorithm>
#include "skiplist.hpp"
using namespace std;

int main() {
    cout<<"\x1b[H\x1b[J";
    skiplist<int, int> sl; int m, n; string s;
    while(1) {
        cout<<"> "; cin>>s;
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        if(s == "insert") {
            cin>>m>>n;
            sl.insert(m, n);
        } else if(s == "select") {
            cin>>m;
            cout<<sl.select(m)<<endl;
        } else if(s == "delete") {
            cin>>m;
            sl.remove(m);
        } else if(s == "print") sl.print();
        else if(s == "clear") cout<<"\x1b[H\x1b[J";
        else if(s == "exit") break;
        else cout<<"\x1b[1;31mInvalid command\x1b[0m\xa";
    } cout<<"\x1b[H\x1b[J";

    return 0;
}