#include<iostream>
#include<vector>
#include<algorithm>

#define INF (1<<29)

using namespace std;

struct LinkCutTree {
  vector<int>left,right,parent,val,mini,minId,lazy;

  LinkCutTree(int n):left(n,-1),right(n,-1),parent(n,-1),val(n,0),
		     mini(n,INF),minId(n,-1),lazy(n,0){}
  
  void update(int id){
    mini[id]=val[id],minId[id]=id;
    
    if(left[id]>=0 && mini[id]>mini[left[id]])
      mini[id]=mini[left[id]],minId[id]=minId[left[id]];
    if(right[id]>=0 && mini[id]>=mini[right[id]])
      mini[id]=mini[right[id]],minId[id]=minId[right[id]];
  }
  
  void apply(int id, int v){ lazy[id]+=v, val[id]+=v, mini[id]+=v; }
  
  void push(int id){
    if(left[id]>=0)apply(left[id],lazy[left[id]]);
    if(right[id]>=0)apply(right[id],lazy[right[id]]);
    lazy[id]=0;
  }
  
  bool is_root(int id){
    return parent[id]<0 || (left[parent[id]]!=id && right[parent[id]]!=id);
  }

  void connect(int ch, int p, bool isL){
    (isL ? left : right)[p] = ch;
    if(ch >= 0)parent[ch] = p;
  }

  void rotate(int id){
    int p = parent[id], q = parent[p];
    push(p),push(id);
    
    bool isL = id==left[p], isRoot = is_root(p);

    connect((isL ? right : left)[id], p, isL);
    connect(p, id, !isL);

    if(!isRoot)connect(id,q,p==left[q]);
    else parent[id]=q;

    update(q);
  }

  void splay(int id){
    while(!is_root(id)){
      int p = parent[id];
      if(!is_root(p))rotate( (id==left[p])^(p==left[parent[p]]) ? p : id );
      rotate(id);
    }
    push(id),update(id);
  }

  int expose(int id){
    int last = -1;
    for(int y=id; y>=0; y=parent[y])
      splay(y), left[y]=last, last=y;
    splay(id);
    return last;
  }

  void link(int ch, int p){
    expose(ch);
    if(right[ch]>=0)return;
    parent[ch]=p;
  }

  void cut(int id){
    expose(id);
    if(right[id]<0)return;
    parent[right[id]]=-1;
    right[id]=-1;
  }

  int lca(int ch, int p){
    expose(ch);
    return expose(p);
  }
};

int main(void){
  int n;
  cin >> n;

  LinkCutTree tree(n);
  
  for(int i=0;i<n;i++){
    int k,t;
    cin >> k;
    for(int j=0;j<k;j++){
      cin >> t;
      tree.link(t,i);
    }
  }
  int q;
  cin >> q;
  for(int i=0;i<q;i++){
    int u,v;
    cin >> u >> v;
    cout << tree.lca(u,v) << endl;
  }
   
  return 0;
}
