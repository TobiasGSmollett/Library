/*
  RollingHash
  See : https://topcoder.g.hatena.ne.jp/spaghetti_source/20130209/1360403866
*/

//#include <bits/stdc++.h>
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>

#define REP(i, n) for (int i=0;i<int(n);++i)
#define FOR(i, a, b) for (int i=int(a);i<int(b);++i)
#define DWN(i, b, a) for (int i=int(b-1);i>=int(a);--i)
#define DO(n) int n____ = n; while(n____--)
#define EACH(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)
#define TO(i, a, b) int s_=a<b?1:-1,b_=b+s_;for(int i=a;i!=b_;i+=s_)
#define SQZ(i, j, a, b) for (int i=int(a),j=int(b)-1;i<j;++i,--j)
#define REP2(i, j, n, m) REP(i, n) REP(j, m)
#define ALL(A) (A).begin(),(A).end()
#define SZ(A) int(A.size())
#define PB push_back

#pragma comment(linker, "/STACK:36777216")
#pragma GCC optimize ("O2")

using namespace std;

typedef unsigned long long ull;

struct RollingHash{
  static const ull p=100000007;
  string s;
  int n;
  vector<ull> pow, phash;
  RollingHash(string s) : s(s), n(SZ(s)), pow(n+1), phash(n+1){
    pow[0]=1,phash[0]=0;
    REP(i, n) {
      phash[i+1] = s[i] + phash[i] * p;
      pow[i+1] = pow[i] * p;
    }
  }

  bool operator()(int i, int j) { 
    int k=lcp(i,j);
    return i+k >= n ? true : j+k >= n ? false : s[i+k] <= s[j+k];
  }
  
  static ull hash(const string &t) {
    ull h=0;
    REP(i,SZ(t))h=t[i]+h*p;
    return h;
  }  

  inline ull hash(int b, int e){
    return phash[e]-phash[b]*pow[e-b];
  }
  
  int find(string t) {
    int w=t.size(),count=0;
    if(w > SZ(s))return 0;
    ull h=hash(t);
    REP(i,n-w+1)if(hash(i,i+w)==h)count++;
    return count;
  }
  
  int lcp(int i, int j){
    int l=0,r=min(n-i,n-j)+1;
    while(r-l>1) {
      int m=(l+r)/2;
      (hash(i,i+m) == hash(j,j+m) ? l : r) = m;
    }
    return l;
  }
  
};

vector<int> suffixArray(const RollingHash &rh){
  vector<int> sa(rh.n);
  REP(i,rh.n)sa[i]=i;
  sort(ALL(sa),rh);
  return sa;
}

vector<int> lcpArray(const vector<int> &sa, RollingHash &rh){
  int sz=max(SZ(sa)-1,0);
  vector<int>res(sz);
  REP(i,sz)res.PB(rh.lcp(sa[i],sa[i+1]));
  return res;  
}

//Wrong "aba"
int longestPalindrome(RollingHash &rh){
  int l=0,r=rh.n;
  while(r-l>1){
    int m=(l+r)/2;
    bool fg=false;
    DWN(i,rh.n,m)fg|=rh.hash(i-m,i)==rh.hash(rh.n-i-1,rh.n-i-1+m);
    (fg?l:r)=m;
  }
  return l;
}
  
  

//POJ 2217 TLE
int main(void){
  ios::sync_with_stdio(false);
  /*
  int n;
  string s,t;
  cin >> n;
  cin.ignore();
  while(n--){
    getline(cin,s);
    getline(cin,t);
    int sl=SZ(s),ans=0;
    RollingHash rh=RollingHash(s+"\0"+t);
    vector<int>sa=suffixArray(rh);

    for(int i=0;i+1<rh.n;i++){
      if((sa[i]<sl)!=(sa[i+1]<sl))ans=max(ans,rh.lcp(sa[i],sa[i+1]));
    }
    
    cout << "Nejdelsi spolecny retezec ma delku " << ans << ".\n";
  }
  */
  string s;
  while(cin >> s){
    RollingHash rh = RollingHash(s);
    cout << longestPalindrome(rh) << "\n";
  }
  return 0;
}
