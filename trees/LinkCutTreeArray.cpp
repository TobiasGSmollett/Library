#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>

#define INF (1<<29)

using namespace std;

struct LinkCutTree {
  vector<int>left,right,parent,val,mini,minId,lazy;
  vector<bool>rev;
  
  LinkCutTree(int n):left(n,-1),right(n,-1),parent(n,-1),val(n,0),
		     mini(n,INF),minId(n,-1),lazy(n,0),rev(n,false){}

  void push_sub(int id, int ch){
    if(ch>=0)lazy[ch]+=lazy[id];
    if(ch>=0 && rev[id])rev[ch]=!rev[ch];
  }
  
  void push(int id){
    int l=left[id], r=right[id];
    push_sub(id,l), push_sub(id,r), rev[id] = false;
    swap(left[id],right[id]);
    val[id]+=lazy[id], mini[id]+=lazy[id], lazy[id]=0;
  }

  void update_min(int id, int ch){
    if(mini[id]>mini[ch])mini[id] = mini[ch], minId[id] = minId[ch];
  }
  
  void update(int id){
    int l=left[id], r=right[id];
    mini[id]=val[id], minId[id]=id, push(id);
    if(l>=0)push(l),update_min(id,l);
    if(r>=0)push(r),update_min(id,r);
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
    update(p);
  }

  void splay(int id){
    while(!is_root(id)){
      int p = parent[id];
      if(!is_root(p))rotate( (id==left[p])^(p==left[parent[p]]) ? p : id );
      rotate(id);
    }
    update(id);
  }

  int expose(int id){
    int last = -1;
    for(int y=id; y>=0; y=parent[y]) splay(y), left[y]=last, last=y;
    splay(id);
    return last;
  }
  int find_root(int id){
    expose(id);
    while(right[id]!=-1)id = right[id];
    return id;
  }
  bool is_connected(int x, int y){
    expose(x), expose(y);
    return (parent[x] != -1);
  }
  void evert(int p){
    expose(p), rev[p] = !rev[p];
  }
  void link(int ch, int p){
    evert(ch), parent[ch]=p;
  }
  void cut(int id){
    expose(id), parent[right[id]] = right[id] = -1;
  }
  int lca(int ch, int p){
    expose(ch);
    return expose(p);
  }
  int getMinId(int id){
    expose(id);
    return minId[id];
  }
  int min(int from, int to){
    evert(from), expose(to);
    return mini[to];
  }
  void add(int id, int val){
    expose(id), lazy[id]=val;
  }
  void add(int from, int to, int v){
    evert(from), expose(to), lazy[to]+=v;
  }
};


bool tree[51][51];

bool getPath(int u,int v,int p,vector<int> &path,int n){
  path.push_back(u);
  if(u==v)return true;
  for(int i=0;i<n;i++)
    if(i!=p && tree[u][i] && getPath(i,v,u,path,n))return true;
  path.pop_back();
  return false;
}

void test(){  
  srand(time(NULL));

  for(int tc=0;tc<1000;tc++){
    int n=abs(rand())%50+1,val[n];

    fill(tree[0],tree[50],0);
    fill(val,val+n,0);
    
    LinkCutTree lctree(n);
    
    for(int q=0;q<1000;q++){
      int com=abs(rand())%3;
      int u=abs(rand())%n,v=abs(rand())%n;
      if(com==0){
	lctree.evert(u);
	lctree.expose(v);
	if (lctree.left[v] == u && lctree.left[u] == -1 && lctree.right[u] == -1) {
	  lctree.cut(v);
	  tree[u][v] = tree[v][u] = false;
	}
      }
      else if(com==1){
	if(lctree.is_connected(u,v)){
	  vector<int>path;
	  getPath(u,v,-1,path,n);
	  int mini = INF;
	  for(int i=0;i<path.size();i++)mini=min(mini,val[path[i]]);

	  if(lctree.min(u,v)!=mini){
	    cout << "assert bitween " << u << " " << v  << " min(u,v) " << lctree.min(u,v) << " mini " << mini << " path.size() "<<path.size()<< endl;
	    assert(false);
	  }
	}
      }
      else if(com==2){
	if(lctree.is_connected(u,v)){
	  vector<int>path;
	  getPath(u,v,-1,path,n);
	  int lazy = abs(rand())%100+1;
	  for(int i=0;i<path.size();i++)val[path[i]]+=lazy;
	  lctree.add(u,v,lazy);
	}
      }
      else {
	if(!lctree.is_connected(u,v)){
	  //cout <<"connect "<< x->id << " " << y->id << endl;
	  lctree.link(u,v);
	  tree[u][v]=tree[v][u]=true;
	}
      }
    }
  }
}

int main(void){
  test();
}
