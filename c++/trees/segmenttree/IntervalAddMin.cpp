#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>

#define INF (1<<29)

using namespace std;

typedef long long ll;

class SegmentTree {
  static const int MAX_N = (1<<18);

  int n,value[MAX_N],delta[MAX_N],size;

  void push(int k){
    value[k] += delta[k];
    if(k+1 < size){
      delta[k*2+1] += delta[k];
      delta[k*2+2] += delta[k];
    }
    delta[k]=0;
  }

  void add(int a,int b,int v,int k, int l, int r){
    push(k);
    if(r <= a || b <= l)return;
    if(a <= l && r <= b)delta[k]+=v,push(k);
    else {
      add(a, b, v, k*2+1, l, (l+r)/2);
      add(a, b, v, k*2+2, (l+r)/2, r);
      value[k]=min(value[k*2+1],value[k*2+2]);
    }
  }
  
  int query(int a,int b,int k,int l, int r){
    push(k);
    if(r <= a || b <= l)return INF;
    if(a <= l && r <= b)return value[k];
    return min(query(a,b,k*2+1,l,(l+r)/2),query(a,b,k*2+2,(l+r)/2,r));
  }
  
public :
  SegmentTree(int n):n(n){
    fill(value,value+MAX_N,0);
    fill(delta,delta+MAX_N,0);
    int n_=1;
    while(n_<n)n_*=2;
    size = n_;
  }

  //add v to [a,b)
  void add(int a,int b,int v){ add(a,b,v,0,0,n); }

  //min [a,b)
  int query(int a,int b){ return query(a,b,0,0,n); }
};

//Usage
int main(void){
  int n,q,a,b,c;
  cin >> n >> q;

  SegmentTree range = SegmentTree(n);

  for(int i=0;i<n;i++){
    scanf("%d",&a);
    range.add(i,i+1,a);
  }
  
  while(q--){
    char com;
    scanf(" %c",&com);
    if(com=='Q'){
      scanf("%d %d",&a, &b);
      printf("%d\n",range.query(a-1,b));
    } else {
      scanf("%d %d %d",&a,&b,&c);
      range.add(a-1,b,c);
    }
  }
  
  return 0;
}
