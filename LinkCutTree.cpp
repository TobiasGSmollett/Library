#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

#define INF (1<<29)
#define max_n 10000

using namespace std;

struct node_t{

  node_t *pp,*lp,*rp;
  int id,val,mini,minId,lazy;
  bool rev;

  node_t(int id,int v):id(id),val(v){
    pp=lp=rp=NULL;
    rev=false; 
    lazy=0; 
    update();
  }
   
  inline void update(){
    mini=val,minId=id;
    if(lp && mini>lp->mini)mini=lp->mini,minId=lp->minId;
    if(rp && mini>=rp->mini)mini=rp->mini,minId=rp->minId;
  }
   
  inline void apply(int v){ lazy+=v, val+=v, mini+=v; }
   
  inline void push(){
    if(rev){
      rev=false;
      swap(lp,rp);
      if(lp)lp->rev^=true;
      if(rp)rp->rev^=true;
    }
    if(lp)lp->apply(lazy);
    if(rp)rp->apply(lazy);
    lazy=0;
  }
 
  bool is_root(){
    return !pp || (pp->lp != this && pp->rp != this);
  }
  
  void rotr(){
    node_t *q=pp,*r=q->pp;
    q->push(),push();
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
    q->push(),push();
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
      if(q->is_root()){
	if(q->lp==this)rotr();
	else rotl();
      } else {
	node_t *r=q->pp;
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
    p->update();
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

void cut(node_t *c){
  expose(c);
  node_t *p=c->lp;
  c->lp=NULL;
  p->pp=NULL;
  c->val=INF;
}

void link(node_t *c,node_t *p){
  expose(c);
  expose(p);
  c->pp=p;
  p->rp=c;
  c->update();
}

//Unverified.
void evert(node_t *p){
  expose(p);
  p->rev^=true;
}

//Verified.
int minId(node_t *x){expose(x); return x->minId;}
void add(node_t *x,int val){ expose(x); x->apply(val); }

//Verified. AOJ GRL_5_C
node_t *LCA(node_t *x,node_t* y){
  expose(x);
  return expose(y);
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
    cout << LCA(node[u],node[v])->id << endl;
  }
  
  return 0;
}
*/
