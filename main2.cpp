#include <iostream>
#include "bf.hpp"

string strlower(string &s) {
    int n = s.length();
    for(int i = 0 ; i < n ; i++) if(isupper(s[i])) s[i] = tolower(s[i]);
    return s;
}

int main() {
    bf<string> b;
    string s, t;
    while(1) {
        cout<<"> "; cin>>s;
        if(strlower(s) == "insert") {
            cin>>t;
            b.insert(t);
        } else if (strlower(s) == "check") {
            cin>>t;
            cout<<(b.lookup(t) ? "yes" : "no")<<endl;
        }
    }
    return 0;
}