/*
  MO’s Algorithm (Query square root decomposition)

  Given an array of size N. All elements of array <= N. 
  You need to answer M queries. Each query is of the 
  form L, R. You need to answer the count of values in 
  range [L, R] which are repeated at least 3 times.

  O(Sqrt(N) * N)

  See:
  http://linkis.com/blog.anudeep2011.com/qeeO4
  https://www.hackerearth.com/notes/mos-algorithm/
*/

#include <bits/stdc++.h>
#define REP(i, n) for (int i=0;i<int(n);++i)
#define ALL(A) A.begin(), A.end()
#define DO(n) int n____ = n; while(n____--)
#define SZ(A) int(A.size())
#define EACH(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)
#define PB push_back

using namespace std;

typedef pair<int,int>pii;

int N, BLOCK_SIZE; //BLOCK_SIZE=sqrt(N);
vector<int> A;

struct Query {
  int id,L,R;

  //Mo's order
  bool operator < (Query q) const {
    int l1=L/BLOCK_SIZE,l2=q.L/BLOCK_SIZE;
    return (l1!=l2) ? l1 < l2 : R<q.R;
  }
};

int cnt[10000],ans;
void add(int pos){cnt[pos]++; if(cnt[pos]==3)ans++; }
void remove(int pos){cnt[pos]--; if(cnt[pos]==2)ans--; }

vector<int> Mo(vector<Query> q){
  int mo_left=0,mo_right=0;
  vector<pii>res;
  
  sort(ALL(q));

  EACH(it,q){
    Query e=(*it);
    while(mo_right < e.R)add(A[mo_right++]);
    while(mo_right > e.R)remove(A[mo_right--]);
    while(mo_left > e.L)add(A[mo_left--]);
    while(mo_left < e.L)remove(A[mo_left++]);
    res.PB(pii(e.id,ans));
  }
  
  sort(ALL(res));
  vector<int>ret;
  EACH(it,res)ret.PB(it->second);
  
  return ret;
}

int main(void){
  ios::sync_with_stdio(false);

  cin >> N;
  BLOCK_SIZE = sqrt(N);
  DO(N){
    int a;
    cin >> a;
    A.PB(a);
  }

  int Q;
  cin >> Q;
  vector<Query>q(Q);
  REP(i,Q){
    cin >> q[i].L >> q[i].R;
    q[i].id=i;
  }

  vector<int>res=Mo(q);

  REP(i,Q)cout << res[i] << "\n";
  
  return 0;
}
