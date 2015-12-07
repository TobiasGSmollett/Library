/*
  RollingHash
  See : https://topcoder.g.hatena.ne.jp/spaghetti_source/20130209/1360403866
*/
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>
#include<cstdio>
#include<cassert>

#define REP(i, n) for (int i=0;i<int(n);++i)
#define ALL(A) (A).begin(),(A).end()
#define SZ(A) int(A.size())

using namespace std;

typedef unsigned long long ull;

struct RollingHash{
  static const ull p=100000007;
  string s;
  int n;
  vector<ull> pow, phash;

  RollingHash(const string &s) : s(s), n(SZ(s)),pow(n+1),phash(n+1){
    pow[0]=1,phash[0]=0;
    REP(i, n) {
      phash[i+1] = s[i] + phash[i] * p;
      pow[i+1] = pow[i] * p;
    }
  }

  inline bool operator()(const int& i, const int& j) const {
    const int k=lcp(i,j);
    return i+k >= n ? true : j+k >= n ? false : s[i+k] <= s[j+k];
  }

  static ull hash(const string &t) {
    ull h=0;
    REP(i,SZ(t))h=t[i]+h*p;
    return h;
  }

  inline ull hash(const int& b, const int& e) const {
    return phash[e]-phash[b]*pow[e-b];
  }

  inline int find(string t) {
    int w=t.size(),count=0;
    if(w > SZ(s))return 0;
    ull h=hash(t);
    REP(i,n-w+1)count+=(hash(i,i+w)==h);
    return count;
  }

  inline int lcp(const int& i, const int& j) const {
    int l=0,r=n-max(i,j)+1;
    while(l+1<r) {
      const int m=(l+r)>>1;
      (hash(i,i+m) == hash(j,j+m) ? l : r) = m;
    }
    return l;
  }
};

int L[1000000],R[1000000];

inline void merge(int *A, const int left, const int mid, const int right, const RollingHash &rh){
  const int n1 = mid - left, n2 = right - mid;
  REP(i,n1)L[i]=A[left+i];
  REP(i,n2)R[i]=A[mid+i];
  for(int k =left,i=0,j=0;k<right;k++){
    if(n2<=j)A[k]=L[i++];
    else if(n1<=i)A[k]=R[j++];
    else A[k]=rh(L[i],R[j])?L[i++]:R[j++];
  }
}

void mergeSort(int *A,const int left,const int right, const RollingHash &rh){
  if(left+1<right){
    const int mid = (left+right)>>1;
    mergeSort(A,left,mid, rh);
    mergeSort(A,mid,right, rh);
    merge(A,left,mid,right, rh);
  }
}

//O(n (log n)^2)
vector<int> suffixArray(const RollingHash &rh){
  int n=rh.n+1,sa[n];
  REP(i,n)sa[i]=i;
  mergeSort(sa,0,n,rh);
  vector<int>res(n);
  REP(i,n)res[i]=sa[i];
  return res;
}

bool contain(const string &S, const vector<int> &sa, const string &T){
  int a=0, b=SZ(S);
  while(b-a>1){
    const int c=(a+b)/2;
    (S.compare(sa[c],T.length(),T)<0 ? a : b ) = c;
  }
  return S.compare(sa[b], T.length(), T) == 0;
}

//POJ 2217 Verified
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
