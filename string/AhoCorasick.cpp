#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

struct AhoCorasick {
  
  static const int ALPHABET_SIZE = 26;
  
  int nodeCount;
  
  struct Node {
    int parent, suffLink;
    int children[ALPHABET_SIZE], transitions[ALPHABET_SIZE];
    char charFromParent;
    bool leaf;
    
    Node(){
      suffLink=-1;
      fill(children,children+ALPHABET_SIZE,-1);
      fill(transitions,transitions+ALPHABET_SIZE,-1);
    }
  };
  
  vector<Node> nodes;

  AhoCorasick(int maxNodes) {
    nodes.resize(maxNodes);
    // create root
    nodes[0].suffLink = 0;
    nodes[0].parent = -1;
    nodeCount = 1;
  }

  void addString(string s) {
    int cur = 0, sl=s.size();
    for(int i=0;i<sl;i++){
      char ch = s[i];
      int c = ch - 'a';
      if(!~nodes[cur].children[c]){
	nodes[nodeCount].parent = cur;
	nodes[nodeCount].charFromParent = ch;
	nodes[cur].children[c] = nodeCount++;
      }
      cur = nodes[cur].children[c];
    }
    nodes[cur].leaf = true;
  }

  int suffLink(int id) {
    Node node = nodes[id];
    if(node.suffLink == -1)
      node.suffLink = !node.parent ? 0 : transition(suffLink(node.parent), node.charFromParent);
    return node.suffLink;
  }

  int transition(int id, char ch) {
    int c = ch - 'a';
    Node node = nodes[id];
    if(!~node.transitions[c])
      node.transitions[c] = ~node.children[c] ? node.children[c] : (!id ? 0 : transition(suffLink(id), ch));
    return node.transitions[c];
  } 
};

int main(void){
  AhoCorasick ahoCorasick(1000);
  ahoCorasick.addString("bc");
  ahoCorasick.addString("abc");
  
  string s = "tabcbc";
  int node = 0;
  vector<int> positions;
  for(int i = 0; i < s.length(); i++) {
    node = ahoCorasick.transition(node, s[i]);
    if(ahoCorasick.nodes[node].leaf)positions.push_back(i);
  }

  for(int i=0;i<positions.size();i++)cout << positions[i] << " ";
  cout << endl;
  return 0;
}
