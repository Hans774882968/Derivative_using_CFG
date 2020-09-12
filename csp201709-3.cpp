#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include <queue>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
//#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
#define rep(i,a,b) for(int i = (a);i <= (b);++i)
const int SZ = 100 + 5;

int n,m;
string s,cur;char token;int idx;
map<string,string> mp;

string parse(string prefixs);
string clean_str(string s);

template<typename Type>inline void read(Type &xx){
    Type f = 1;char ch;xx = 0;
    for(ch = getchar();ch < '0' || ch > '9';ch = getchar()) if(ch == '-') f = -1;
    for(;ch >= '0' && ch <= '9';ch = getchar()) xx = xx * 10 + ch - '0';
    xx *= f;
}

string myreplace(string s,string old,string cur){
    int pos = 0;string ret;
    while(pos < s.size()){
        int pos1 = s.find(old,pos);
        if(pos1 == string::npos){ret += s.substr(pos);break;}
        else ret += s.substr(pos,pos1 - pos) + cur,pos = pos1 + old.size();
    }
    return ret;
}

void advance(){
    if(s[idx] == ',' || s[idx] == ':' || s[idx] == '{' || s[idx] == '}'){
        token = s[idx++];
        return;
    }
    if(idx > 1 && s[idx - 1] == '"' && s[idx - 2] != '\\'){
        cur = "";
        while(1){
            cur += s[idx++];
            //这种写法不能正确处理"\\\\\\\\"这种字符串。if(s[idx] == '"' && s[idx - 1] != '\\') break;
            if(s[idx] == '"' && (s[idx + 1] == ',' || s[idx + 1] == '}' || s[idx + 1] == ':')) break;
        }
    }
    else token = s[idx++];
}

string parse_str(){
    advance();//assert(token == '"');
    advance();
    string ret = cur;
    advance();//assert(token == '"');
    return ret;
}

string parse_val(string prefixs){
    if(s[idx] == '{') return parse(prefixs);
    return clean_str(parse_str());
}

string parse_key(){
    return parse_str();
}

string clean_str(string s){
    s = myreplace(s,"\\\\","\\");
    s = myreplace(s,"\\\"","\"");
    return s;
}
//key必须预先clean，放进map后再clean比较麻烦；
//val可以预先clean，但是对象不应该被clean，因为会产生未预料结果。比如(假设已转义)"\\\\"->"\\"->"\"。
string parse(string prefixs){
    int tidx = idx;
    advance();//assert(token == '{');
    string ky = clean_str(parse_key());
    advance();//assert(token == ':');
    mp[prefixs + ky] = parse_val(prefixs + ky + ".");
    advance();
    while(token == ','){
        ky = clean_str(parse_key());
        advance();//assert(token == ':');
        mp[prefixs + ky] = parse_val(prefixs + ky + ".");
        advance();
    }
    //assert(token == '}');
    return s.substr(tidx,idx - tidx);
}

int main(int argc, char** argv) {
    read(n);read(m);
    rep(i,1,n){
        char s0[SZ];fgets(s0,SZ,stdin);s0[strlen(s0) - 1] = '\0';
        string s00 = s0;
        s += myreplace(s00," ","");
    }
    idx = 0;
    parse("");
    while(m--){
        char q[SZ];scanf("%s",q);
        if(!mp.count(q)) puts("NOTEXIST");
        else{
            if(mp[q][0] == '{') puts("OBJECT");
            else cout << "STRING " << mp[q] << endl;
        }
    }
    getchar();getchar();
    return 0;
}