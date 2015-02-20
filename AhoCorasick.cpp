/*
  Unverified.
*/
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

struct AhoCorasick{
  static const int alph_size=26;

  struct node_t{
    int parent,suffLink;
    char charFromParent;
    int children[alph_size];
    int transitions[alph_size];
    bool isLeaf;

    node_t(){
      fill(children,children+alph_size,-1);
      fill(transitions,transitions+alph_size,-1);
      suffLink=-1;
      isLeaf=false;
    }
  };

  vector<node_t>nodes;
  int nodeCount;
  
  AhoCorasick(int maxNodes){
    nodes.resize(maxNodes);
    //make root
    nodes[0]=node_t();
    nodes[0].suffLink=0;
    nodes[0].parent=-1;
    nodeCount=1;
  }

  void add(string s){
    int cur=0;
    for(int i=0;i<s.size();i++){
      int c=s[i]-'a';
      if(nodes[cur].children[c]==-1){
	nodes[nodeCount]=node_t();
	nodes[nodeCount].parent=cur;
	nodes[nodeCount].charFromParent=s[i];
	nodes[cur].children[c]=nodeCount++;
      }
      cur=nodes[cur].children[c];
    }
    nodes[cur].isLeaf=true;
  }

  int suffLink(int nodeIndex){
    node_t node=nodes[nodeIndex];
    if(node.suffLink==-1){
      node.suffLink=(node.parent==0)?0:
	transition(suffLink(node.parent),node.charFromParent);
    }
    return node.suffLink;  
  }

  int transition(int nodeIndex,char ch){
    int c=ch-'a';
    node_t node=nodes[nodeIndex];
    if(node.transitions[c]==-1){
      node.transitions[c]=(node.children[c]!=-1)?node.children[c]:
	(nodeIndex==0?0:transition(suffLink(nodeIndex),ch));
    }
    return node.transitions[c];
  }

  int contain(string s){
    int node=0,cnt=0;
    for(int i=0;i<s.size();i++){
      node=transition(node,s[i]);
      cnt+=nodes[node].isLeaf;
    }
    return cnt;
  }
};

//usage example
int main(void){

  AhoCorasick aho(1000);
  
  aho.add("bc");
  aho.add("bca");
  
  string s="bcabc";

  cout << aho.contain(s) << endl;

  return 0;
}
