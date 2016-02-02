#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>
#include<cstdlib> 
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
      pp=(node_t*)NULL;
      lp=(node_t*)NULL;
      rp=(node_t*)NULL;
    }
  };

  node_t *root;
  
  void rotl(node_t *x){
    node_t *y=x->rp;
    if(x && y)x->rp=y->lp;
    if(y && y->lp)y->lp->pp=x;
    if(x && y)y->pp=x->pp;
    
    if(!x->pp)root=y;
    else if(x==x->pp->lp)x->pp->lp=y;
    else x->pp->rp=y;

    if(y)y->lp=x;
    if(x)x->pp=y;
  }

  void rotr(node_t *x){
    node_t *y=x->lp;

    if(x && y)x->lp=y->rp;
    if(y && y->rp)y->rp->pp=x;
    if(x && y)y->pp=x->pp;

    if(!x->pp)root=y;
    else if(x==x->pp->lp)x->pp->lp=y;
    else x->pp->rp=y;

    if(y)y->rp=x;
    if(x)x->pp=y;
  }

  void insert_fixup(node_t *z){
    node_t *y=(node_t*)NULL;
    
    while(z->pp && z->pp->color==red){
      if(z->pp==z->pp->pp->lp){
	y=z->pp->pp->rp;
	if(y && y->color==red){
	  z->pp->color=black;
	  y->color=black;
	  z->pp->pp->color=red;
	  z=z->pp->pp;
	}
	else if(z==z->pp->rp){
	  z=z->pp;
	  rotl(z);
	}
	else {
	  z->pp->color=black;
	  z->pp->pp->color=red;
	  rotr(z->pp->pp);
	}
      }
      else {
	y=z->pp->pp->lp;
	if(y && y->color==red){
	  z->pp->color=black;
	  y->color=black;
	  z->pp->pp->color=red;
	  z=z->pp->pp;
	}
	else if(z==z->pp->lp){
	  z=z->pp;
	  rotr(z);
	}
	else {
	  z->pp->color=black;
	  z->pp->pp->color=red;
	  rotl(z->pp->pp);
	}
      }
    }
    root->color=black;
  }
  
  void insert(node_t *z){
    node_t *y = (node_t*)NULL, *x = root;
    while(x){
      y=x;
      /*
      if(z->id < x->id)x=x->lp;
      else x=x->rp;
      */
      if(z->id == x->id){
	x->val = z->val;
	return;
      }
      if(z->id < x->id)x=x->lp;
      else x=x->rp;
    }
    z->pp=y;
    if(!y)root=z;
    else if(z->id < y->id)y->lp=z;
    else y->rp=z;
    z->lp=(node_t*)NULL;
    z->rp=(node_t*)NULL;
    z->color=red;
    insert_fixup(z);
  }

  void erase_fixup(node_t *x){
    node_t *w=(node_t*)NULL;
    while(x && x!=root && x->color==black){
      if(x==x->pp->lp){
	w=x->pp->rp;
	if(w && w->color==red){
	  w->color=black;
	  x->pp->color=red;
	  rotl(x->pp);
	  w=x->pp->rp;
	}
	else if(w && w->lp && w->rp && w->lp->color==black && w->rp->color==black){
	  w->color=red;
	  x=x->pp;
	}
	else if(w && w->rp && w->rp->color==black){
	  if(w->lp)w->lp->color=black;
	  w->color=red;
	  rotr(w);
	  w=x->pp->rp;
	}
	else {
	  if(w)w->color=x->pp->color;
	  x->pp->color=black;
	  if(w && w->rp)w->rp->color=black;
	  rotl(x->pp);
	  x=root;
	}
      }
      else {
	w=x->pp->lp;
	if(w && w->color==red){
	  w->color=black;
	  x->pp->color=red;
	  rotr(x->pp);
	  w=x->pp->lp;
	}
	else if(w && w->rp && w->lp && w->rp->color==black && w->lp->color==black){
	  w->color=red;
	  x=x->pp;
	}
	else if(w && w->lp && w->lp->color==black){
	  if(w->rp)w->rp->color=black;
	  w->color=red;
	  rotl(w);
	  w=x->pp->lp;
	}
	else {
	  if(w)w->color=x->pp->color;
	  x->pp->color=black;
	  if(w && w->lp)w->lp->color=black;
	  rotr(x->pp);
	  x=root;
	}
      }
    }
    if(x)x->color=black;
  }

  void transplant(node_t *u, node_t *v){
    if(!u->pp)root=v;
    else if(u==u->pp->lp)u->pp->lp=v;
    else u->pp->rp=v;
    if(v)v->pp=u->pp;
  }

  node_t *tree_minimum(node_t *x){
    while(x->lp)x=x->lp;
    return x;
  }
  
  void erase(node_t *z){
    if(!z)return;

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
      y=tree_minimum(z->rp);
      y_original_color=y->color;
      x=y->rp;
      if(y->pp==z && x)x->pp=y;
      else {
	transplant(y,y->rp);
	y->rp=z->rp;
	if(y->rp)y->rp->pp=y;
      }
      transplant(z,y);
      y->lp=z->lp;
      y->lp->pp=y;
      y->color=z->color;
    }

    if(y_original_color==black)erase_fixup(x);
    cout << "b" << endl;
    delete z;
  }

  node_t *find(int id, node_t *x){
    while(x){
      if(id == x->id)return x;
      if(id < x->id)x=x->lp;
      else x=x->rp;
    }
    return (node_t*)NULL;
  }
  
public:

  RedBlackTree(){
    root=(node_t*)NULL;
  }

  int find(int id){
    node_t *res=find(id,root);
    return (!res) ? -1 : res->val;
  }
  
  void insert(int id,int val){
    insert(new node_t(id,val));
  }

  void erase(int id){
    node_t *tmp = find(id,root);
    if(tmp)erase(tmp);
  }
};

RedBlackTree tree;
void test(){
  srand(time(NULL));
  vector<int>v(1000,-1);
  int tc=8000;
  while(tc--){
    int q=rand()%3;
    cout << q << endl;
    if(q==0){
      int id=rand()%1000,val=rand()%100;
      if(v[id]==-1){
	tree.insert(id,val);
	v[id]=val;
      }
    }    
    else if(q==1){
      int id=rand()%1000;
      tree.erase(id);
      v[id]=-1;
    }
    else {
      int id=rand()%1000;
      int a=tree.find(id),b=v[id];
      if(a!=b)cout << a << " " << b << endl;
      assert(a==b);
    }
  }
}

void test2(){
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
}

int main(){
  test();
  return 0;
}
