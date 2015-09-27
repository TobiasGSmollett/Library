#include <bits/stdc++.h>
#define REP(i, n) for (int i=0;i<int(n);++i)
#define ALL(A) A.begin(), A.end()
#define DO(n) int n____ = n; while(n____--)
#define SZ(A) int(A.size())
#define EACH(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)
#define TRV(A) __typeof(A[0])e=A[0]; for(int i=0;i!=int(A.size());e=A[++i])

using namespace std;

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

vector<int> Mo(vector<Query> q){
  int mo_left=0,mo_right=-1;
  
  sort(ALL(q));

  TRV(q){
    while(mo_right<e.R){mo_right++;}
    while(mo_right>e.R){mo_right--;}
    while(mo_left>e.L){mo_left--;}
    while(mo_left<e.L){mo_left++;}
  }
  
}

int main(void){

return 0;
}
