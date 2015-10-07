#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<cassert>

using namespace std;

struct AhoCorasick {

  static const int ALPHABET_SIZE = 2, root=0;

  int N; // num of node

  struct Node {
    int parent, link;
    int child[ALPHABET_SIZE], next[ALPHABET_SIZE];
    char ch; //from parent
    bool leaf;

    Node():parent(0),link(0),ch('\0'),leaf(false){
      link=-1;
      fill(child,child+ALPHABET_SIZE,-1);
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
      int c = ch - '0';
      if(!~nodes[cur].child[c]){
	nodes[N].parent = cur;
	nodes[N].ch = ch;
	nodes[cur].child[c] = N++;
      }
      cur = nodes[cur].child[c];
    }
    nodes[cur].leaf = true;
  }

  int link(int id) {
    Node node = nodes[id];
    if(node.link == -1){
      if(!node.parent)node.link=root;
      else node.link=trans(link(node.parent),node.ch);
    }
    return node.link;
  }

  int trans(int id, char ch) {
    int c = ch - '0';
    Node node = nodes[id];
    if(!~node.next[c]){
      if(~node.child[c])node.next[c]=node.child[c];
      else if(!id)node.next[c]=root;
      else node.next[c]=trans(link(id),ch);
    }
    return node.next[c];
  }
};

typedef pair<int,int> pii;

vector<pii> bakerBird(const vector<string>&T, const vector<string>&pattern){
  int pl=pattern.size(),tl=T.size(),pil=pattern[0].size();
  AhoCorasick aho(1000000);

  for(int i=0;i<pl;i++){
    aho.addString(pattern[i]);
  }
   

  vector<int>acc;
  for(int i=0;i<pl;i++){
    int node = 0;
    for(int j=0;j<pil;j++){
      node = aho.trans(node, pattern[i][j]);
      if(aho.nodes[node].leaf)acc.push_back(node);
    }
  }

  const int til = T[0].size();
    
  vector<vector<int> >td(til);
  for(int i=0;i<tl;i++){
    int node = 0;
    for(int j=0;j<til;j++){
      node = aho.trans(node, T[i][j]);
      td[til-j-1].push_back(node);
    }
  }

  vector<pii>res;
  int tl2=acc.size(),sl=acc.size()+tl+1;
  vector<int>a(acc.size()+tl+2,-1);
    
  for(int i=0;i<til;i++){
    vector<int>s=acc;
    s.push_back(-1);
    for(int j=0;j<til;j++)s.push_back(td[i][j]);
    for(int k=0,j=-1;k<sl;a[++k]=++j)while(j>=0 && s[k]!=s[j])j=a[j];
    for(int k=tl2+1;k<=sl;k++){
      if(a[k]==tl2)res.push_back(pii(k-tl2*2-1,til-i-pil));
    }
  }
  return res;
}

int main(void){
  ios::sync_with_stdio(false);
  
  int h,w,r,c;
  
  cin >> h >> w;
  vector<string>s(h);
  for(int i=0;i<h;i++)cin >> s[i];
  
  cin >> r >> c;
  vector<string>t(r);
  for(int i=0;i<r;i++)cin >> t[i];
  
  vector<pii>res=bakerBird(s,t);
  sort(res.begin(),res.end());

  for(int i=0;i<res.size();i++)
    cout << res[i].first << " " << res[i].second << "\n";

  return 0;
}
