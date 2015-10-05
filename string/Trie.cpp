#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

/*
  Unverified.
*/
struct Trie{
  
private:
  struct node_t{
    node_t* children[128];
    bool isLeaf;
    node_t(){fill(children,children+128,(node_t*)NULL);}
  };

  node_t *root;

  void print(string s, node_t *t){
    for(int ch=0;ch<128;ch++){
      node_t *child=t->children[ch];
      if(child)print(s+(char)ch,child);
    }
    if(t->isLeaf)cout << s << endl;
  }

  void insert(string s,node_t *t){
    node_t *v=t;
    for(int i=0;i<s.size();i++){
      node_t *nx=v->children[s[i]];
      if(!nx)v->children[s[i]]=nx=new node_t();
      v=nx;
    }
    v->isLeaf=true;
  }

  void clear(node_t *t){
    for(int i=0;i<128;i++){
      if(t->children[i])clear(t->children[i]);
    }
    delete t;
  }

public:
  Trie(){root=new node_t();}
  void insert(string s){insert(s,root);}
  void print(){print("",root);}
  void clear(){clear(root);root=new node_t();}
};

int main(void){
  
  return 0;
}
