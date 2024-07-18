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
// #include <bits/stdc++.h>
using namespace std;
typedef long long LL;
#define rep(i, a, b) for (int i = (a); i <= (b); ++i)
const int SZ = 100 + 5;

int n, m;
string s, cur;
char token;
int idx;
map<string, string> mp;

string parse(string prefixes);
string clean_str(string s);

template <typename Type>
inline void read(Type& xx) {
  Type f = 1;
  char ch;
  xx = 0;
  for (ch = getchar(); ch < '0' || ch > '9'; ch = getchar())
    if (ch == '-') f = -1;
  for (; ch >= '0' && ch <= '9'; ch = getchar()) xx = xx * 10 + ch - '0';
  xx *= f;
}

// regex_replace不能正确匹配若干反斜杠这种字符串
string my_replace(string s, string old, string cur) {
  int pos = 0;
  string ret;
  while (pos < s.size()) {
    int pos1 = s.find(old, pos);
    if (pos1 == string::npos) {
      ret += s.substr(pos);
      break;
    } else
      ret += s.substr(pos, pos1 - pos) + cur, pos = pos1 + old.size();
  }
  return ret;
}

// 预处理。仍需要保证反斜杠后面的字符为‘"’或‘\’
string clear_spaces(string s) {
  string ret = "";
  int pos = 0;
  while (pos < s.size()) {
    int tmpp = pos;
    for (; pos < s.size() && s[pos] != '"'; ++pos);
    ret += my_replace(s.substr(tmpp, pos - tmpp), " ", "");
    if (pos >= s.size()) break;
    tmpp = pos;
    int slash_state = 0;
    for (; pos < s.size(); ++pos) {
      if (s[pos] == '\\')
        slash_state = !slash_state;
      else if (pos > tmpp && s[pos] == '"') {
        if (!slash_state)
          break;
        else
          slash_state = 0;
      }
    }
    ++pos;
    ret += s.substr(tmpp, pos - tmpp);
  }
  return ret;
}

void advance() {
  /*
  思想：归纳基础是可以正确来到第1个引号。那么我们只要保证反斜杠是匹配的，就可以保证第1次正确地来到和上述引号匹配的引号。这个算法可以保证第1个key正确读取，于是由归纳法，当前总是能正确判定是否在字符串内部。
  最后应停在匹配引号的后一个位置，比如":"、","和"}"。
  */
  if (s[idx] == '"' && s[idx - 1] != '\\') {
    int pos = idx, slash_state = 0;
    for (;; ++pos) {
      if (s[pos] == '\\')
        slash_state = !slash_state;
      else if (pos > idx && s[pos] == '"') {
        if (!slash_state)
          break;
        else
          slash_state = 0;
      }
    }
    cur = s.substr(idx + 1, pos - idx - 1);
    idx = pos + 1;
  } else
    token = s[idx++];
}

string parse_str() {
  advance();
  return cur;
}

string parse_val(string prefixes) {
  if (s[idx] == '{') return parse(prefixes);
  return clean_str(parse_str());
}

string parse_key() { return parse_str(); }

string clean_str(string s) {
  s = my_replace(s, "\\\\", "\\");
  s = my_replace(s, "\\\"", "\"");
  return s;
}

// key必须预先clean，放进map后再clean比较麻烦；
// val可以预先clean，但是对象不应该被clean，因为会产生未预料结果。比如(假设已转义)"\\\\"->"\\"->"\"。
string parse(string prefixes) {
  int tidx = idx;
  advance();  // assert(token == '{');
  string ky = clean_str(parse_key());
  advance();  // assert(token == ':');
  mp[prefixes + ky] = parse_val(prefixes + ky + ".");
  advance();
  while (token == ',') {
    ky = clean_str(parse_key());
    advance();  // assert(token == ':');
    mp[prefixes + ky] = parse_val(prefixes + ky + ".");
    advance();
  }
  // assert(token == '}');
  return s.substr(tidx, idx - tidx);
}

int main(int argc, char** argv) {
  read(n);
  read(m);
  rep(i, 1, n) {
    char s0[SZ];
    fgets(s0, SZ, stdin);
    s0[strlen(s0) - 1] = '\0';
    string s00 = s0;
    s += s00;
  }
  s = clear_spaces(s);
  idx = 0;
  parse("");
  while (m--) {
    char q[SZ];
    scanf("%s", q);
    if (!mp.count(q))
      puts("NOTEXIST");
    else {
      if (mp[q][0] == '{')
        puts("OBJECT");
      else
        cout << "STRING " << mp[q] << endl;
    }
  }
  getchar();
  getchar();
  return 0;
}
