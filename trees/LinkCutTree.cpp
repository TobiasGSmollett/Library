#include<bits/stdc++.h>

#define INF (1<<29)
#define max_n 10000

using namespace std;

struct node_t{
  node_t *pp,*lp,*rp;
  int id,val,mini,minId,lazy;
  bool rev;

  node_t(int id,int v):id(id),val(v),mini(v){
    pp=lp=rp=NULL;
    rev=false; 
    lazy=0; 
    update();
  }
   
  void update(){
    mini=val,minId=id;
    push();
    if(lp)lp->push();
    if(rp)rp->push();
    if(lp && mini>lp->mini)mini=lp->mini,minId=lp->minId;
    if(rp && mini>rp->mini)mini=rp->mini,minId=rp->minId;
  }
   
  void push(){
    if(rev){
      rev=false;
      swap(lp,rp);
      if(lp)lp->rev^=true;
      if(rp)rp->rev^=true;
    }
    if(lp)lp->lazy+=lazy;
    if(rp)rp->lazy+=lazy;

    val+=lazy,mini+=lazy,lazy=0;
  }
 
  bool is_root(){
    return !pp || (pp->lp != this && pp->rp != this);
  }
  
  void rotr(){
    node_t *q=pp,*r=q->pp;
    if((q->lp=rp))rp->pp=q;
    rp=q;q->pp=this;
    if((pp=r)){
      if(r->lp==q)r->lp=this;
      if(r->rp==q)r->rp=this;
    }
    q->update();
  }

  void rotl(){
    node_t *q=pp,*r=q->pp;
    if((q->rp=lp))lp->pp=q;
    lp=q;q->pp=this;
    if((pp=r)){
      if(r->lp==q)r->lp=this;
      if(r->rp==q)r->rp=this;
    }
    q->update();
  }

  void splay(){
    while(!is_root()){
      node_t *q=pp;
      node_t *r=q->pp;
      if(!q->is_root())r->push();
      q->push();
      push();
      if(q->is_root()){
	if(q->lp==this)rotr();
	else rotl();
      } else {
	if(r->lp==q){
	  if(q->lp==this){q->rotr();rotr();}
	  else {rotl();rotr();}
	} else {
	  if(q->rp==this){q->rotl();rotl();}
	  else {rotr();rotl();}
	}
      }
    }
    push();
    update();
  }
};

node_t *expose(node_t *x){
  node_t *rp=NULL;
  for(node_t *p=x;p;p=p->pp){
    p->splay();
    p->rp=rp;
    rp=p;
  }
  x->splay();
  return rp;
}

node_t *find_root(node_t *x){
  expose(x);
  while(x->lp)x=x->lp;
  return x;
}

bool isConnected(node_t *x, node_t *y){
  if(x->id==y->id)return true;
  expose(x);
  //assert(x->pp==NULL);
  expose(y);
  return (x->pp != NULL);
}

//Verified.
void evert(node_t *p){
  expose(p);
  p->rev^=true;
}

void cut(node_t *c){
  expose(c);
  node_t *p=c->lp;
  c->lp=NULL;
  p->pp=NULL;
}

void link(node_t *c,node_t *p){
  if(isConnected(c,p))return;
  evert(c);
  c->pp=p;
}

//Verified.
int minId(node_t *x){expose(x); return x->minId;}
void add(node_t *x,int val){ expose(x); x->lazy=val; }

//Verified. AOJ GRL_5_C
node_t *lca(node_t *x,node_t* y){
  expose(x);
  return expose(y);
}

//Verified.
int min(node_t *from, node_t *to){
  evert(from);
  expose(to);
  return to->mini;
}

//Verified.
void add(node_t *from, node_t *to,int v){
  evert(from);
  expose(to);
  to->lazy+=v;
}

/*
//AOJ GRL_5_C
node_t *node[100001];

int main(void){
  int n;
  cin >> n;
  
  for(int i=0;i<n;i++)node[i]=new node_t(i,0);
  for(int i=0;i<n;i++){
    int k,t;
    cin >> k;
    for(int j=0;j<k;j++){
      cin >> t;
      link(node[t],node[i]);
    }
  }

  int q;
  cin >> q;
  for(int i=0;i<q;i++){
    int u,v;
    cin >> u >> v;
    cout << lca(node[u],node[v])->id << endl;
  }
  
  return 0;
}
*/

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

    node_t *node[n];
    for(int i=0;i<n;i++)node[i] = new node_t(i,0);

    fill(tree[0],tree[50],0);
    fill(val,val+n,0);
    
    for(int q=0;q<1000;q++){
      int com=abs(rand())%10;
      int u=abs(rand())%n,v=abs(rand())%n;
      node_t *x=node[u],*y=node[v];

      //cout << com << " " << u << " " << v << endl;
      
      if(com==0){
	evert(x);
	expose(y);
	if (y->lp == x && x->lp == NULL && x->rp == NULL) {
	  cut(y);
	  tree[u][v] = tree[v][u] = false;
	}
      }
      else if(com==1){
	if(isConnected(x,y)){
	  vector<int>path;
	  getPath(u,v,-1,path,n);
	  int mini = INF;
	  for(int i=0;i<path.size();i++)mini=min(mini,val[path[i]]);
	  if(min(x,y)!=mini){
	    //cout << "assert bitween " << u << " " << v  << " min(x,y) " << min(x,y) << " mini " << mini << " path.size() "<<path.size()<< endl;
	    assert(false);
	  }
	}
      }
      else if(com==2){
	if(isConnected(x,y)){
	  vector<int>path;
	  getPath(u,v,-1,path,n);
	  int lazy = abs(rand())%100+1;
	  for(int i=0;i<path.size();i++)val[path[i]]+=lazy;
	  add(x,y,lazy);
	}
      }
      else {
	if(!isConnected(x,y)){
	  //cout <<"connect "<< x->id << " " << y->id << endl;
	  link(x,y);
	  tree[u][v]=tree[v][u]=true;
	}
      }
    }
  }
}

int main(void){
  test();
}
