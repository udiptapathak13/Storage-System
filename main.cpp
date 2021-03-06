#include <iostream>
#include "lsm.hpp"

using namespace std;

string strlower(string &s) {
    int n = s.length();
    for(int i = 0 ; i < n ; i++) if(isupper(s[i])) s[i] = tolower(s[i]);
    return s;
}

int main() {
    cout<<"\x1b[H\x1b[J";
    lsm<string, string> l;
    string s, key, val;
    
    while(1) {
        cout<<"> "; cin>>s;
        if(strlower(s) == "insert" || s == "update") {
            cin>>key>>val;
            l.insert(key, val);
        }
        else if(s == "select") {
            cin>>key;
            val = l.select(key).first;
            cout<<'+';
            for(int i = -2 ; i < (int) key.length() ; i++) 
                cout<<"-";
            cout<<'+';
            for(int i = -2 ; i < (int) val.length() ; i++) 
                cout<<"-";
            cout<<"+\xa| ";
            cout<<key<<" | "<<val;
            cout<<" |\xa+";
            for(int i = -2 ; i < (int) key.length() ; i++) 
                cout<<"-";
            cout<<'+';
            for(int i = -2 ; i < (int) val.length() ; i++) 
                cout<<"-";
            cout<<"+\xa";
        } else if(s == "delete") {
            cin>>key;
            l.remove(key);
        } else if (s == "clear") cout<<"\x1b[H\x1b[J";
        else if (s == "exit") break;
        else cout<<"\x1b[1;31mInvalid command\x1b[0m\xa";
    } cout<<"\x1b[H\x1b[J";
    return 0;
}