#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
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
typedef map<string, LL> mp_type;
#define rep(i, a, b) for (int i = (a); i <= (b); ++i)
const int SZ = 1e3 + 5;

int idx, n;
char s[SZ];
int val;
string nam;
char token;

template <typename Type>
inline void read(Type &xx) {
  Type f = 1;
  char ch;
  xx = 0;
  for (ch = getchar(); ch < '0' || ch > '9'; ch = getchar())
    if (ch == '-') f = -1;
  for (; ch >= '0' && ch <= '9'; ch = getchar()) xx = xx * 10 + ch - '0';
  xx *= f;
}

mp_type get_ad();
mp_type ele();

void advance() {
  if (idx >= n) return;
  if (s[idx] >= '0' && s[idx] <= '9') {
    val = 0;
    for (; idx < n && s[idx] >= '0' && s[idx] <= '9'; ++idx)
      val = 10 * val + s[idx] - '0';
    return;
  } else if (isupper(s[idx])) {
    nam = s[idx++];
    if (islower(s[idx])) nam += s[idx++];
    return;
  }
  token = s[idx++];
}

mp_type get_ad() {
  mp_type mpe;
  if (s[idx] == '(') {
    advance();
    assert(token == '(');
    mpe = ele();
    advance();
    assert(token == ')');
  } else {
    advance();
    mpe[nam] = 1;
  }
  int K;
  if (s[idx] >= '0' && s[idx] <= '9')
    advance(), K = val;
  else
    K = 1;
  for (auto &x : mpe) x.second *= K;
  return mpe;
}

void map_add(mp_type &mpe, mp_type &mpe0) {
  for (auto &x : mpe0) {
    if (!mpe.count(x.first))
      mpe[x.first] = x.second;
    else
      mpe[x.first] += x.second;
  }
}

mp_type ele() {
  mp_type mpe = get_ad();
  while (idx < n && (s[idx] == '(' || isalpha(s[idx]))) {
    mp_type mpe0 = get_ad();
    map_add(mpe, mpe0);
  }
  return mpe;
}

mp_type get_exp() {
  int K;
  if (s[idx] >= '0' && s[idx] <= '9')
    advance(), K = val;
  else
    K = 1;
  mp_type mpe = ele();
  for (auto &x : mpe) x.second *= K;
  return mpe;
}

mp_type expre() {
  mp_type mpe = get_exp();
  while (idx < n && s[idx] == '+') {
    advance();
    mp_type mpe2 = get_exp();
    map_add(mpe, mpe2);
  }
  return mpe;
}

bool eq() {
  mp_type mp0 = expre();
  advance();
  assert(token == '=');
  mp_type mp1 = expre();
  return mp0 == mp1;
}

int main(int argc, char **argv) {
  int T;
  read(T);
  while (T--) {
    idx = 0;
    scanf("%s", s);
    n = strlen(s);
    puts(eq() ? "Y" : "N");
  }
  return 0;
}
