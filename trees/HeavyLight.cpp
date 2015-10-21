#include<iostream>
#include<vector>
#include<algorithm>

#define REP(i, n) for (int i=0;i<int(n);++i)
#define EACH(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)
#define INF (1<<29)

using namespace std;

typedef long long ll;

class SegmentTree {
  static const int MAX_N = (1<<18);

  int n,value[MAX_N],delta[MAX_N],size;

  void push(int k){
    value[k] += delta[k];
    if(k+1 < size){
      delta[k*2+1] += delta[k];
      delta[k*2+2] += delta[k];
    }
    delta[k]=0;
  }

  void add(int a,int b,int v,int k, int l, int r){
    push(k);
    if(r <= a || b <= l)return;
    if(a <= l && r <= b)delta[k]+=v,push(k);
    else {
      add(a, b, v, k*2+1, l, (l+r)/2);
      add(a, b, v, k*2+2, (l+r)/2, r);
      value[k]=min(value[k*2+1],value[k*2+2]);
    }
  }

  int query(int a,int b,int k,int l, int r){
    push(k);
    if(r <= a || b <= l)return INF;
    if(a <= l && r <= b)return value[k];
    return min(query(a,b,k*2+1,l,(l+r)/2),query(a,b,k*2+2,(l+r)/2,r));
  }
  
public:
  
  SegmentTree(int n):n(n){
    fill(value,value+MAX_N,0);
    fill(delta,delta+MAX_N,0);
    int n_=1;
    while(n_<n)n_*=2;
    size = n_;
  }

  void add(int a, int v){ add(a,a+1,v,0,0,n); }
  void add(int a,int b,int v){ add(a,b,v,0,0,n); }
  int query(int a,int b){ return query(a,b,0,0,n); }
};

struct Node {
  int id,val,parent,size,in,out;
  vector<int> children;
  
  Node():val(0){}
  Node(int id,int v,vector<int>ch):id(id),val(v),children(ch){}
};

typedef vector<Node> Tree;

struct HeavyLight {
  int pathCount,n;
  vector<int>path, pathSize, pathPos, pathRoot;
  Tree tree;
  vector<SegmentTree> paths;

  HeavyLight(Tree tree)
    :pathCount(0),n(tree.size()),path(n),pathSize(n),pathPos(n),pathRoot(n),tree(tree){
    int time=0;
    dfs(0,-1,time);
    buildPaths(0,newPath(0));

    REP(i,pathCount)paths.push_back(SegmentTree(2*pathSize[i]));
    REP(i,n)paths[path[i]].add(pathPos[i],tree[i].val);
  }

  void dfs(int u, int p, int &k){
    tree[u].in=k++, tree[u].parent=p, tree[u].size=1;
    EACH(v,tree[u].children)
      if(*v!=p)dfs(*v, u, k),tree[u].size += tree[*v].size;
    tree[u].out=k++;
  }

  int newPath(int u){ pathRoot[pathCount] = u; return pathCount++; }

  void buildPaths(int u, int pt){
    path[u]=pt, pathPos[u]=pathSize[pt]++;
    EACH(v,tree[u].children)
      if(*v != tree[u].parent)
	buildPaths(*v, 2*tree[*v].size >= tree[u].size ? pt : newPath(*v));
  }

  int queryPath(int pathId, int from, int to){
    return paths[pathId].query(from,to+1);
  }

  void addPath(int pathId,int from,int to,int val){
    paths[pathId].add(from,to+1,val);
  }
  
#define up(a,b) for(int r; !isAncestor(r = pathRoot[path[a]], b); a = tree[r].parent)
  
  int query(int a, int b) {
    int res = INF;
    up(a,b)res = min(res, queryPath(path[a], 0, pathPos[a]));
    up(b,a)res = min(res, queryPath(path[b], 0, pathPos[b]));
    return min(res,queryPath(path[a], min(pathPos[a], pathPos[b]),max(pathPos[a], pathPos[b])));
  }

  void modify(int a, int b, int del){
    up(a,b)addPath(path[a],0,pathPos[a],del);
    up(b,a)addPath(path[b],0,pathPos[b],del);
    addPath(path[a],min(pathPos[a], pathPos[b]),max(pathPos[a], pathPos[b]),del);
  }
  
  bool isAncestor(int p, int ch){ return tree[p].in <= tree[ch].in && tree[ch].out <= tree[p].out; }

  int lca(int a,int b){
    up(a,b);
    up(b,a);
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
    int com,u,v,w;
    cin >> com;
    if(com==1){
      cin >> u >> v >> w;
      hl.modify(u,v,w);
    }
    else {
      cin >> u >> v;
      cout << hl.query(u,v) << "\n";
    }
  }
  return 0;
}
