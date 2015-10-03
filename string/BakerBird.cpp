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
      leaf=false;
      fill(children,children+ALPHABET_SIZE,-1);
      fill(transitions,transitions+ALPHABET_SIZE,-1);
    }
  };
  
  vector<Node> nodes;
  
  AhoCorasick(int maxNodes){
    nodes.resize(maxNodes);
    nodes[0].suffLink = 0;
    nodes[0].parent = -1;
    nodeCount = 1;
  }

  void addString(string s){
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

  int suffLink(int id){
    Node node = nodes[id];
    if(node.suffLink == -1)
      node.suffLink = !node.parent ? 0 : transition(suffLink(node.parent), node.charFromParent);
    return node.suffLink;
  }

  int transition(int id, char ch){
    int c = ch - 'a';
    Node node = nodes[id];
    if(!~node.transitions[c])
      node.transitions[c] = ~node.children[c] ? node.children[c] : (!id ? 0 : transition(suffLink(id), ch));
    return node.transitions[c];
  } 
};

int bakerBird(const vector<string>&T, const vector<string>&pattern){
  int pl=pattern.size(),tl=T.size(),cnt=0;
  AhoCorasick aho(1000);
  for(int i=0;i<pl;i++)aho.addString(pattern[i]);
  string acc;
  for(int i=0;i<pl;i++){
    int node = 0, pil = pattern[i].size();
    for(int j=0;j<pil;j++){
      node = aho.transition(node, pattern[i][j]);
      if(aho.nodes[node].leaf)acc+=(node+'0');
    }
  }

  vector<string>td(tl);
  for(int i=0;i<tl;i++){
    int node = 0, til = T[i].size();
    for(int j=0;j<til;j++){
      node = aho.transition(node, T[i][j]);
      td[til-j-1]+=(node+'0');
    }  
  }

  for(int i=0;i<tl;i++){
    string s=acc+"\1"+td[i];
    int sl=s.size(),al=acc.size();
    vector<int>a(s.size(),-1);
    for(int j=0,k=-1;j<sl;a[++j]=++k)while(k>=0 && s[j]!=s[k])k=a[k];
    for(int j=tl+1;j<=sl;j++)cnt+=(a[j]==al);
  }
  return cnt;
}

int main(void){
  int n,m;
  cin >> n >> m;
  vector<string>s(n),t(m);
  for(int i=0;i<n;i++)cin >> s[i];
  for(int i=0;i<m;i++)cin >> t[i];

  cout << bakerBird(t,s) << endl;
  
  return 0;
}
