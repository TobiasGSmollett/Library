/*
  RollingHash
  See : https://topcoder.g.hatena.ne.jp/spaghetti_source/20130209/1360403866
*/
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>

#define REP(i, n) for (int i=0;i<int(n);++i)
#define ALL(A) (A).begin(),(A).end()
#define SZ(A) int(A.size())
#define PB push_back

using namespace std;

typedef unsigned long long ull;

struct RollingHash{
  static const ull p=100000007;
  string s;
  int n;
  vector<ull> pow, ph;
  
  RollingHash(string s) : s(s), n(SZ(s)), pow(n+1), ph(n+1){
    pow[0]=1,ph[0]=0;
    REP(i, n)ph[i+1]=s[i]+ph[i]*p,pow[i+1]=pow[i]*p;
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
    return ph[e]-ph[b]*pow[e-b];
  }
  
  inline int find(string t) {
    int w=t.size(),count=0;
    if(w > SZ(s))return 0;
    ull h=hash(t);
    REP(i,n-w+1)count+=(hash(i,i+w)==h);
    return count;
  }
  
  inline int lcp(int i, int j){
    int l=0,r=min(n-i,n-j)+1;
    while(r-l>1){
      int m=(l+r)/2;
      (hash(i,i+m) == hash(j,j+m) ? l : r) = m;
    }
    return l;
  }
};

vector<int> suffixArray(const RollingHash &rh){
  vector<int> sa(rh.n+1);
  REP(i,rh.n+1)sa[i]=i;
  sort(ALL(sa),rh);
  return sa;
}

vector<int> lcpArray(const vector<int> &sa, RollingHash &rh){
  int sz=max(SZ(sa)-1,0);
  vector<int>res(sz);
  REP(i,sz)res.PB(rh.lcp(sa[i],sa[i+1]));
  return res;  
}

//POJ 2217 TLE
int main(void){
  ios::sync_with_stdio(false);

  int n;
  string s,t;
  cin >> n;
  cin.ignore();
  while(n--){
    getline(cin,s);
    getline(cin,t);
    int sl=SZ(s),ans=0;
    RollingHash rh=RollingHash(s+"\1"+t);
    vector<int>sa=suffixArray(rh);

    for(int i=0;i+1<rh.n;i++){
      if((sa[i]<sl)!=(sa[i+1]<sl))ans=max(ans,rh.lcp(sa[i],sa[i+1]));
    }
    
    cout << "Nejdelsi spolecny retezec ma delku " << ans << ".\n";
  }

  return 0;
}
