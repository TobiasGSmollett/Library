#include<iostream>
#include<vector>
#include<algorithm>

#define INF (1<<29)
#define max_n 10000

using namespace std;

struct node_t;

struct edge{
  node_t *p;
  int attr;
  edge():p(NULL),attr(0){}
  edge(node_t *p):p(p),attr(0){}
  edge(int v):p(NULL),attr(v){}
  edge(node_t *p,int v):p(p),attr(v){}

  void clear(){p=NULL,attr=0;}
};

struct node_t{
  edge p,l,r;
  int id,val,mini,minId,lazy;
  bool rev;

  node_t(int id,int v):id(id),val(v),mini(v){
    rev=false; 
    lazy=0; 
    update();
  }
   
  void update(){
    mini=val,minId=id;
    push();
    if(l.p)l.p->push();
    if(r.p)r.p->push();
    if(l.p && mini>l.p->mini)mini=l.p->mini,minId=l.p->minId;
    if(r.p && mini>r.p->mini)mini=r.p->mini,minId=r.p->minId;
  }
   
  void push(){
    if(rev){
      rev=false;
      swap(l,r);
      if(l.p)l.p->rev^=true;
      if(r.p)r.p->rev^=true;
    }
    if(l.p)l.p->lazy+=lazy;
    if(r.p)r.p->lazy+=lazy;

    val+=lazy,mini+=lazy,lazy=0;
  }
 
  bool is_root(){
    return !p.p || (p.p->l.p != this && p.p->r.p != this);
  }
  
  void rotr(){
    node_t *q=p.p,*tr=q->p.p;
    if((q->l.p=r.p))r.p->p.p=q;
    r.p=q;q->p.p=this;
    if((p.p=tr)){
      if(tr->l.p==q)tr->l.p=this;
      if(tr->r.p==q)tr->r.p=this;
    }
    q->update();
  }

  void rotl(){
    node_t *q=p.p,*tr=q->p.p;
    if((q->r.p=l.p))l.p->p.p=q;
    l.p=q;q->p.p=this;
    if((p.p=tr)){
      if(tr->l.p==q)tr->l.p=this;
      if(tr->r.p==q)tr->r.p=this;
    }
    q->update();
  }

  void splay(){
    while(!is_root()){
      node_t *q=p.p;
      node_t *tr=q->p.p;
      if(!q->is_root())tr->push();
      q->push();
      push();
      if(q->is_root()){
	if(q->l.p==this)rotr();
	else rotl();
      } else {
	if(tr->l.p==q){
	  if(q->l.p==this){q->rotr();rotr();}
	  else {rotl();rotr();}
	} else {
	  if(q->r.p==this){q->rotl();rotl();}
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
  for(node_t *p=x;p;p=p->p.p){
    p->splay();
    p->r.p=rp;
    rp=p;
  }
  x->splay();
  return rp;
}

node_t *findRoot(node_t *x){
  expose(x);
  while(x->l.p)x=x->l.p;
  return x;
}

bool same(node_t *x, node_t *y){
  if(x->id==y->id)return true;
  expose(x), expose(y);
  return (x->p.p != NULL);
}

void evert(node_t *p){
  expose(p);
  p->rev^=true;
}

void cut(node_t *c){
  expose(c);
  node_t *p=c->l.p;
  c->l.clear(), p->p.clear();
}

void link(node_t *c,node_t *p){
  if(same(c,p))return;
  evert(c), c->p=edge(p);
}

void link(node_t *c, node_t *p, int v){
  if(same(c,p))return;
  evert(c), c->p=edge(p,v);
}

int minId(node_t *x){
  expose(x); return x->minId;
}

void add(node_t *x,int val){
  expose(x), x->lazy=val;
}

node_t *lca(node_t *x,node_t* y){
  expose(x);
  return expose(y);
}

int min(node_t *from, node_t *to){
  evert(from), expose(to);
  return to->mini;
}

void add(node_t *from, node_t *to,int v){
  evert(from), expose(to), to->lazy+=v;
}

int main(void){

  return 0;
}
