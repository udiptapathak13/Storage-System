#include <iostream>
#include "slsm.hpp"

using namespace std;

void perform(slsm<string, string>& l) {
}

int main() {
    cout<<"\x1b[H\x1b[J";
    slsm<string, string> l;
    string s, key, val;
    
    while(1) {
        cout<<"> "; cin>>s;
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        if(s == "insert" || s == "update") {
            cin>>key>>val;
            l.insert(key, val);
        }
        else if(s == "select") {
            cin>>key;
            val = l.select(key);
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
        } else if (s == "perform") {
            perform(l);
        }else if (s == "clear") cout<<"\x1b[H\x1b[J";
        else if (s == "exit") break;
        else cout<<"\x1b[1;31mInvalid command\x1b[0m\xa";
    } cout<<"\x1b[H\x1b[J";
    return 0;
}