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

typedef pair<int,int> pii;

vector<pii> bakerBird(const vector<string>&T, const vector<string>&pattern){
  int pl=pattern.size(),tl=T.size(),pil=pattern[0].size();
  AhoCorasick aho(1000);
  
  for(int i=0;i<pl;i++)aho.addString(pattern[i]);
  string acc;
  
  for(int i=0;i<pl;i++){
    int node = 0;
    for(int j=0;j<pil;j++){
      node = aho.transition(node, pattern[i][j]);
      if(aho.nodes[node].leaf)acc+=(node+'0');
    }
  }
  
  const int til = T[0].size();
    
  vector<string>td(til);
  for(int i=0;i<tl;i++){
    int node = 0;
    for(int j=0;j<til;j++){
      node = aho.transition(node, T[i][j]);
      td[til-j-1]+=(node+'0');
    }
  }

  vector<pii>res;
  int tl2=acc.size(),sl=acc.size()+tl+1;
  vector<int>a(acc.size()+tl+2,-1);
    
  for(int i=0;i<til;i++){
    string s=acc+"\1"+td[i];
    for(int k=0,j=-1;k<sl;a[++k]=++j)while(j>=0 && s[k]!=s[j])j=a[j];
    for(int k=tl2+1;k<=sl;k++){
      if(a[k]==tl2)res.push_back(pii(k-tl2*2-1,til-i-pil));
    }
  }
  return res;
}

int main(void){
  int n,m;
  cin >> n >> m;
  vector<string>s(n),t(m);
  for(int i=0;i<n;i++)cin >> s[i];
  for(int i=0;i<m;i++)cin >> t[i];

  vector<pii> res=bakerBird(t,s);

  for(int i=0;i<res.size();i++)
    cout << res[i].first << " " << res[i].second << endl;
      
  return 0;
}
