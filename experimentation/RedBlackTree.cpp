#include<iostream>
#include<vector>
#include<algorithm>

#define INF (1<<29)

using namespace std;

class RedBlackTree{

  static const bool black=false,red=true;

  struct node_t{
    bool color;
    int id,val;
    node_t *pp, *lp, *rp;

    node_t(int id,int v):id(id),val(v){
      color=black;
      pp=lp=rp=0;
    }
  };

  node_t *root;
  
  void rotl(node_t *x){
    node_t *y=x->lp;
    x->rp=y->lp;
    if(y->lp)y->lp->pp=x;
    y->pp=x->pp;
    if(!x->pp)root=y;
    else if(x==x->pp->lp)x->pp->lp=y;
    else x->pp->rp=y;
    y->lp=x;
    x->pp=y;
  }

  void rotr(node_t *x){
    node_t *y=x->rp;
    x->lp=y->rp;
    if(y->rp)y->rp->pp=x;
    y->pp=x->pp;
    if(!x->pp)root=y;
    else if(x==x->pp->rp)x->pp->rp=y;
    else x->pp->lp=y;
    y->rp=x;
    x->pp=y;
  }
  
  int val(node_t *t){return t->val;}

  void insert_fixup(node_t *z){
    while(z->pp && z->pp->color==red){
      node_t *y=0;
      if(z->pp==z->pp->pp->lp){
	y=z->pp->pp->rp;
	if(y->color==red){
	  z->pp->color=black;
	  y->color=black;
	  z->pp->pp->color=red;
	  z=z->pp->pp;
	}
	else if(z==z->pp->rp){
	  z=z->pp;
	  rotl(z);
	}
	z->pp->color=black;
	z->pp->pp->color=red;
	rotr(z->pp->pp);
      }
      else {
	y=z->pp->pp->lp;
	if(y->color==red){
	  z->pp->color=black;
	  y->color=black;
	  z->pp->pp->color=red;
	  z=z->pp->pp;
	}
	else if(z==z->pp->lp){
	  z=z->pp;
	  rotr(z);
	}
	z->pp->color=black;
	z->pp->pp->color=red;
	rotl(z->pp->pp);
      }
    }
    root->color=black;
  }
  
  void insert(node_t *z){
    node_t *y = 0, *x = root;
    while(x){
      y=x;
      if(z->id < x->id)x=x->lp;
      else x=x->rp;
    }
    z->pp=y;
    if(!y)root=z;
    else if(z->id < y->id)y->lp=z;
    else y->rp=z;
    z->lp=0;
    z->rp=0;
    z->color=red;
    insert_fixup(z);
  }

  void transplant(node_t *u, node_t *v){

    if(!u->pp)root=v;
    else if(u==u->pp->lp)u->pp->lp=v;
    else u->pp->rp=v;
    if(u->pp)v->pp=u->pp;
  }

  void erase_fixup(node_t *x){
    while(x!=root && x->color==black){
      node_t *w=0;
      if(x==x->pp->lp){
	w=x->pp->rp;
	if(w->color==red){
	  w->color=black;
	  x->pp->color=red;
	  rotl(x->pp);
	  w=x->pp->rp;
	}
	if(w->lp->color==black && w->rp->color==black){
	  w->color=red;
	  x=x->pp;
	}
	else if(w->rp->color==black){
	  w->lp->color=black;
	  w->color=red;
	  rotr(w);
	  w=x->pp->rp;
	}
	w->color=x->pp->color;
	x->pp->color=black;
	w->rp->color=black;
	rotl(x->pp);
	x=root;
      }
      else {
	w=x->pp->lp;
	if(w->color==red){
	  w->color=black;
	  x->pp->color=red;
	  rotr(x->pp);
	  w=x->pp->lp;
	}
	if(w->rp->color==black && w->lp->color==black){
	  w->color=red;
	  x=x->pp;
	}
	else if(w->lp->color==black){
	  w->rp->color=black;
	  w->color=red;
	  rotl(w);
	  w=x->pp->lp;
	}
	w->color=x->pp->color;
	x->pp->color=black;
	w->lp->color=black;
	rotr(x->pp);
	x=root;
      }
    }
  }
  
  void erase(node_t *z){

    node_t *x=0,*y=z;
    bool y_original_color=y->color;

    if(!z->lp){
      x=z->rp;
      transplant(z,z->rp);
    }
    else if(!z->rp){
      x=z->lp;
      transplant(z,z->lp);
    }
    else {
      y_original_color=y->color;
      x=y->rp;
      if(y->pp==z)x->pp=y;
      else {
	transplant(y,y->rp);
	y->rp=z->rp;
	y->rp->pp=y;
      }
      transplant(z,y);
      y->lp=z->lp;
      y->lp->pp=y;
      y->color=z->color;
    }
    if(y_original_color==black)erase_fixup(x);
    delete z;
  }

  node_t *find(int id, node_t *x){
    if(!x)return 0;
    if(x->id == id)return x;
    else if(x->id < id)return find(id,x->rp);
    else return find(id, x->lp);
  }
  
public:

  int find(int id){
    node_t *res=find(id,root);
    return res ? res->val : -1;
  }
  
  void insert(int id,int val){
    insert(new node_t(id,val));
  }

  void erase(int id){
    erase(find(id,root));
  }
};

int main(void){
  RedBlackTree tree;
  int q,id,val;
  while(cin >> q){
    if(q==0){
      cin >> id >> val;
      tree.insert(id,val);
    }
    else if(q==1){
      cin >> id;
      tree.erase(id);
    }
    else {
      cin >> id;
      cout << tree.find(id) << endl;
    }
  }

  return 0;
}
