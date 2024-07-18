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
#include <algorithm>
// #include <bits/stdc++.h>
using namespace std;
typedef long long LL;
#define rep(i, a, b) for (int i = (a); i <= (b); ++i)
const int SZ = 1e6 + 5;

int idx;
char s[9];

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

int advance() {
  if (idx >= 7) return -1;
  char ch = s[idx++];
  if (ch >= '0' && ch <= '9') return ch - '0';
  return ch;
}

int E() {
  int x1 = advance();
  while (idx < 7 && (s[idx] == 'x' || s[idx] == '/')) {
    char op = (char)advance();
    int x2 = advance();
    if (op == 'x')
      x1 *= x2;
    else
      x1 /= x2;
  }
  return x1;
}

int S() {
  int x1 = E();
  while (idx < 7) {
    char op = (char)advance();
    int x2 = E();
    if (op == '+')
      x1 += x2;
    else
      x1 -= x2;
  }
  return x1;
}

int main(int argc, char** argv) {
  int T;
  read(T);
  while (T--) {
    idx = 0;
    scanf("%s", s);
    int ans = S();
    puts(ans == 24 ? "Yes" : "No");
  }
  return 0;
}
