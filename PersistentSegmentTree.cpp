#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>

using namespace std;

typedef long long ll;

/*
  Verified. NPCA Judge #97
*/
struct node_t{
  int num;
  node_t *lch,*rch;
  
  node_t(){}  
  node_t(node_t *l,node_t *r,int n):num(n),lch(l),rch(r){}
  node_t(node_t *l,node_t *r):lch(l),rch(r){
    num=(l?l->num:0)+(r?r->num:0);
  }
};

struct PersistentSegmentTree{
private:
  static node_t *root[200005]; //root[number_of_query]
  static node_t tree[5000000];

  int numOfNode,now,cur;
  node_t *nil;
  
  node_t *newNode(node_t *lch,node_t *rch,int num){
    return &(tree[cur++]=node_t(lch,rch,num));
  }
  
  node_t *newNode(node_t *lch,node_t *rch){
    return &(tree[cur++]=node_t(lch,rch));
  }

  node_t *update(int id, int l,int r, node_t *t){
    if(r-l<=1)return newNode(nil,nil,t->num+1);
    int m=(l+r)/2;
    if(id<m)return newNode(update(id,l,m,t->lch),t->rch);
    else return newNode(t->lch,update(id,m,r,t->rch));
  }

  int query(int a,int b,int l,int r,node_t *t){
    if(t==nil)return 0;
    if(b<=l || r<=a)return 0;
    if(a<=l && r<=b)return t->num;
    int m=(l+r)/2;
    return query(a,b,l,m,t->lch)+query(a,b,m,r,t->rch);
  }

  public:
  PersistentSegmentTree(int n):numOfNode(n),now(0),cur(0){
    root[0]=nil=newNode(0,0,0);
    nil->lch=nil->rch=nil;
  }

  void update(int id){
    root[now+1]=update(id,0,numOfNode,root[now]);
    now++;
  }

  int query(int a,int b,int x){
    return query(a,b,0,numOfNode,root[x]);
  }
};
node_t PersistentSegmentTree::tree[5000000] = {};
node_t* PersistentSegmentTree::root[200005] = {};


/*
//NPCA Judge #97
int main(void){

  int n;
  cin >> n;
  vector<int>l(n),r(n);

  for(int i=0;i<n;i++)cin >> l[i];
  for(int i=0;i<n;i++)cin >> r[i];

  PersistentSegmentTree t(n);

  int x=0;
  for(int i=0;i<n;i++){
    t.update(x);
    ll cnt=t.query(l[i],r[i],x+1);
    x=(cnt*l[i]+r[i])%(i+2);
  }

  cout << x << endl;
  
  return 0;
}
*/
