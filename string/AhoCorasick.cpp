#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

struct AhoCorasick {
  
  static const int ALPHABET_SIZE = 26, root=0;
  
  int N; // num of node
  
  struct Node {
    int parent, link;
    int children[ALPHABET_SIZE], next[ALPHABET_SIZE];
    char charFromParent;
    bool leaf;
    
    Node(){
      link=-1;
      fill(children,children+ALPHABET_SIZE,-1);
      fill(next,next+ALPHABET_SIZE,-1);
    }
  };
  
  vector<Node> nodes;

  AhoCorasick(int maxNodes):N(1),nodes(maxNodes){
    nodes[root].link = root;
    nodes[root].parent = -1;
  }

  void addString(string s) {
    int cur = 0, sl=s.size();
    for(int i=0;i<sl;i++){
      char ch = s[i];
      int c = ch - 'a';
      if(!~nodes[cur].children[c]){
	nodes[N].parent = cur;
	nodes[N].charFromParent = ch;
	nodes[cur].children[c] = N++;
      }
      cur = nodes[cur].children[c];
    }
    nodes[cur].leaf = true;
  }

  int link(int id) {
    Node node = nodes[id];
    if(node.link == -1){
      if(!node.parent)node.link=root;
      else node.link=trans(link(node.parent),node.charFromParent);
    }
    return node.link;
  }

  int trans(int id, char ch) {
    int c = ch - 'a';
    Node node = nodes[id];
    if(!~node.next[c]){
      if(~node.children[c])node.next[c]=node.children[c];
      else if(!id)node.next[c]=root;
      else node.next[c]=trans(link(id),ch);
    }
    return node.next[c];
  } 
};

int main(void){

  return 0;
}
