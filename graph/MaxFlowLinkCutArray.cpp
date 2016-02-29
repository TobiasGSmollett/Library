#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

#define INF (1<<29)
#define max_n 10000

using namespace std;

struct edge{int to,cap,rev;};

struct LinkCutTree {
  vector<int>left,right,parent,val,mini,minId,lazy;
  vector<edge *>edges;
  
  LinkCutTree(int n,int v):left(n,-1),right(n,-1),parent(n,-1),val(n,v),
			   mini(n,INF),minId(n,-1),lazy(n,0),edges(n,NULL){}  
  void push(int id){
    int l=left[id], r=right[id];
    if(l>=0)lazy[l]+=lazy[id];
    if(r>=0)lazy[r]+=lazy[id];
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
    for(int y=id; y>=0; y=parent[y])
      splay(y), left[y]=last, last=y;
    splay(id);
    return last;
  }

  int find_root(int id){
    expose(id);
    while(right[id]!=-1)id = right[id];
    return id;
  }
  
  void link(int ch, int p){
    expose(ch);
    expose(p);
    if(right[ch]>=0)return;
    parent[ch]=p;
    left[p]=ch;
  }
  
  void link(int c,int p,int v,edge *e){
    link(c,p);
    val[c]=v;
    update(c);
    edges[c]=e;
  }
  
  void cut(int id){
    expose(id);
    if(right[id]<0)return;
    parent[right[id]]=-1;
    right[id]=-1;
    val[id]=INF;
  }
  
  int lca(int ch, int p){
    expose(ch);
    return expose(p);
  }
  int getMinId(int id){
    expose(id);
    return minId[id];
  }
  void add(int id, int val){
    expose(id);
    lazy[id]=val;
  }
};

vector<edge> g[max_n];

void add_edge(int from,int to,int cap){
  g[from].push_back((edge){to,cap,(int)g[to].size()});
  g[to].push_back((edge){from,0,(int)g[from].size()-1});
}
 
int dist[max_n];

bool bfs(int s,int t){
  fill(dist,dist+max_n,-1);
  dist[s]=0;
  queue<int>que;
  que.push(s);
  while(!que.empty()){
    int u=que.front();que.pop();
    if(u==t)return true;
    for(int j=0;j<g[u].size();j++){
      edge e=g[u][j];
      if(dist[e.to]<0 && e.cap>0){
	dist[e.to]=dist[u]+1;
	que.push(e.to);
      }
    }
  }
  return false;
}

int n,ptr[max_n];
vector<int>lists[max_n];

bool pour(int id,int i,LinkCutTree* tree){
  int u=lists[id][i];
  if(tree->find_root(u)==u)return true;
  edge *e = tree->edges[u];
  tree->expose(u);
  int df=e->cap - tree->val[u];
  e->cap -= df;
  g[e->to][e->rev].cap += df;
  return false;
}

int max_flow(int s,int t){
  int flow=0;
  while(bfs(s,t)){
    fill(ptr,ptr+max_n,0);
    LinkCutTree tree(n,INF);
    for(int i=0;i<n;i++)lists[i].clear();
    while(true){
      int v = tree.find_root(s);
      if(v==t){
	v=tree.getMinId(s);
	tree.expose(v);
	flow += tree.mini[v];
	tree.add(s,-tree.mini[v]);
	while(true){
	  v=tree.getMinId(s);
	  if(tree.val[v]>0)break;
	  g[tree.edges[v]->to][tree.edges[v]->rev].cap += tree.edges[v]->cap;
	  tree.edges[v]->cap = 0;
	  tree.cut(v);
	}
	continue;
      }

      if(ptr[v] < g[v].size()){
	edge &e=g[v][ptr[v]++];
	if(dist[e.to]==dist[v]+1 && e.cap>0){
	  tree.link(v,e.to,e.cap,&e);
	  lists[e.to].push_back(v);
	} 
      } else {
	if(v==s){
	  for(int i=0;i<n;i++){
	    for(int j=0;j<lists[i].size();j++)pour(i,j,&tree);
	    lists[i].clear();
	  }
	  break;
	}
	for(int i=0;i<lists[v].size();i++){
	  if(!pour(v,i,&tree))tree.cut(lists[v][i]);
	}
	lists[v].clear();
      }     
    }
  }
  return flow;
}

// AOJ GRL_6_A
int main(void){
  
  int e;
  cin >> n >> e;
  for(int i=0;i<e;i++){
    int a,b,c;
    cin >> a >> b >> c;
    add_edge(a,b,c);
  }
 
  cout << max_flow(0,n-1) << endl;;
 
  return 0;
}
