#include<iostream>
#include<vector>
#include<algorithm>

#define REP(i, n) for (int i=0;i<int(n);++i)
#define FOR(i, a, b) for (int i=int(a);i<int(b);++i)
#define EACH(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)
#define ALL(A) A.begin(), A.end()

using namespace std;

struct Node{
  int id,val,parent,size,in,out;
  vector<int> children;
  Node(){}
  Node(int id,int v,vector<int>ch):id(id),val(v),children(ch){}
};

typedef vector<Node> Tree;

struct HeavyLight {
  int pathCount,n;
  vector<int>path, pathSize, pathPos, pathRoot;
  Tree tree;
  
  HeavyLight(Tree tree)
    :pathCount(0),n(tree.size()),path(n),pathSize(n),pathPos(n),pathRoot(n),tree(tree){
    int time=0;
    dfs(0,-1,time);
    buildPaths(0,newPath(0));
  }

  void dfs(int u, int p, int &k){
    tree[u].in=k++, tree[u].parent=p, tree[u].size=1;
    EACH(v,tree[u].children){
      if(*v!=p){
	dfs(*v, u, k);
	tree[u].size += tree[*v].size;
      }
    }
    tree[u].out=k++;
  }

  int newPath(int u){
    pathRoot[pathCount] = u;
    return pathCount++;
  }

  void buildPaths(int u, int pt){
    path[u]=pt, pathPos[u]=pathSize[pt]++;
    EACH(v,tree[u].children)
      if(*v != tree[u].parent)
	buildPaths(*v, 2* tree[*v].size >= tree[u].size ? pt : newPath(*v));
  }

  bool isAncestor(int p, int ch){
    return tree[p].in <= tree[ch].in && tree[ch].out <= tree[p].out;
  }

  int lca(int a,int b){
    for(int root; !isAncestor(root=pathRoot[path[a]],b);a=tree[root].parent);
    for(int root; !isAncestor(root=pathRoot[path[b]],a);b=tree[root].parent);
    return isAncestor(a,b)?a:b;
  }
};


int main(void){
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  Tree tree(n);
  
  REP(i,n){
    tree[i].id=i;
    int k;
    cin >> k;
    REP(j,k){
      int ch;
      cin >> ch;
      tree[i].children.push_back(ch);
    }
  }

  HeavyLight hl = HeavyLight(tree);

  int q;
  cin >> q;
  while(q--){
    int u,v;
    cin >> u >> v;
    cout << hl.lca(u,v) << "\n";
  }
  
  return 0;
}
