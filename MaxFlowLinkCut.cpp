/*
  Verified. 
  O(EV log V)
*/
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<climits>
#include<cassert>
 
#define INF (1<<29)
#define max_n 10000
 
using namespace std;
 
struct edge{int to,cap,rev;};
 
struct node_t{
  node_t *pp, *lp, *rp;
  int id,val,mini,minId,lazy,saved;
  edge *e;
 
  node_t(int id,int v):id(id),val(v){
    pp=lp=rp=NULL; saved=lazy=0; update();
  }
   
  void update(){
    mini=val,minId=id;
    if(lp && mini>lp->mini)mini=lp->mini,minId=lp->minId;
    if(rp && mini>=rp->mini)mini=rp->mini,minId=rp->minId;
  }
   
  void apply(int v){ lazy+=v, val+=v, mini+=v; }
   
  void push(){
    if(lp)lp->apply(lazy);
    if(rp)rp->apply(lazy);
    lazy=0;
  }
 
  bool is_root(){
    return !pp || (pp->lp != this && pp->rp != this);
  }
};
 
void connect(node_t *ch, node_t *p,bool lch){
  (lch?p->lp:p->rp)=ch;
  if(ch)ch->pp=p;
}
 
void rotate(node_t *x){
  node_t *p=x->pp,*g=p->pp;
  p->push(),x->push();
  bool isRootP=p->is_root(),isLch=(x==p->lp);
  connect(isLch?x->rp:x->lp,p,isLch);
  connect(p,x,!isLch);
  if(!isRootP)connect(x,g,p==g->lp);
  else x->pp=g;
  p->update();
}
 
void splay(node_t *x){
  while(!x->is_root()){
    node_t *p=x->pp,*g=p->pp;
    if(!p->is_root())rotate((x==p->lp)==(p==g->lp)?p:x);
    rotate(x);
  }
  x->push();
  x->update();
}
 
node_t *expose(node_t *x){
  node_t *last=NULL;
  for(node_t *y=x;y;y=y->pp) {
    splay(y);
    y->lp=last;
    y->update();
    last=y;
  }
  splay(x);
  return last;
}
 
node_t *findRoot(node_t *x) {
  expose(x);
  while(x->rp)x=x->rp;
  return x;
}
 
void link(node_t *x,node_t *y,int newValue,edge *e) {
  expose(x);
  x->pp=y;
  x->val=newValue;
  x->update();
  x->e=e;
}
 
void cut(node_t *x) {
  expose(x);
  x->rp->pp=NULL;
  x->rp=NULL;
  x->saved=x->val;
  x->val=INF;
}

int minId(node_t *x){ expose(x); return x->minId; }
void add(node_t *x,int lazy){ expose(x); x->apply(lazy); }

vector<edge> g[max_n];
int n;
 
void add_edge(int from,int to,int cap){
  g[from].push_back((edge){to,cap,g[to].size()});
  g[to].push_back((edge){from,0,g[from].size()-1});
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

int ptr[max_n];
node_t *nodes[max_n];
vector<int>lists[max_n];

bool func(int id,int i){
  int u=lists[id][i];
  if(findRoot(nodes[u])==nodes[u])return true;
  edge *e=nodes[u]->e;
  expose(nodes[u]);
  int df=e->cap-(nodes[u]->val<INF?nodes[u]->val:nodes[u]->saved);
  e->cap-=df;
  g[e->to][e->rev].cap+=df;
  return false;
}
 
int max_flow(int S,int T){
  int flow=0;
  while(bfs(S,T)){

    fill(ptr,ptr+max_n,0);
    for(int i=0;i<n;i++)nodes[i]=new node_t(i,INF);
    
    node_t *s=nodes[S],*t=nodes[T];
    for(int i=0;i<n;i++)lists[i].clear();
    
    while(true){
      node_t *v=findRoot(s);

      if(v==t){
	expose(v=nodes[minId(s)]);
	flow+=v->mini;
	add(s,-v->mini);
	while(true){
	  expose(v=nodes[minId(s)]);
	  if(v->val>0)break;
	  g[v->e->to][v->e->rev].cap+=v->e->cap;
	  v->e->cap=0;
	  cut(v);
	}
	continue;
      } 
      
      if(ptr[v->id] < g[v->id].size()){
	edge &e=g[v->id][ptr[v->id]++];
	if(dist[e.to]==dist[v->id]+1 && e.cap>0){
	  link(v,nodes[e.to],e.cap,&e);
	  lists[e.to].push_back(v->id);
	} 
      } else {
	if(v==s){
	  for(int i=0;i<n;i++){
	    for(int j=0;j<lists[i].size();j++)func(i,j);
	    lists[i].clear();
	  }
	  break;
	}
	for(int i=0;i<lists[v->id].size();i++){
	  if(!func(v->id,i))cut(nodes[lists[v->id][i]]);
	}
	lists[v->id].clear();
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
