#include<iostream>
#include<vector>
#include<algorithm>

#define REP(i, n) for (int i=0;i<int(n);++i)
#define FOR(i, a, b) for (int i=int(a);i<int(b);++i)
#define DWN(i, b, a) for (int i=int(b-1);i>=int(a);--i)
#define EACH(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)
#define ALL(A) A.begin(), A.end()

using namespace std;

struct Ring {
  inline static int query(int a, int b){ return max(a,b); }
  inline static int modify(int a, int b){ return a+b; }
  static const int qzero = INT_MIN, mzero = 0;
} ring;

struct Node {
  int id,val,parent,size,in,out;
  vector<int> children;
  
  Node():val(ring.mzero){}
  Node(int id,int v,vector<int>ch):id(id),val(v),children(ch){}
};

typedef vector<Node> Tree;

struct HeavyLight {
  int pathCount,n;
  vector<int>path, pathSize, pathPos, pathRoot;
  Tree tree;
  vector<vector<int> >value, delta, len; //segment tree
  
  HeavyLight(Tree tree)
    :pathCount(0),n(tree.size()),path(n),pathSize(n),pathPos(n),pathRoot(n),tree(tree){
    int time=0;
    dfs(0,-1,time);
    buildPaths(0,newPath(0));
    
    value.resize(pathCount);
    delta.resize(pathCount);
    len.resize(pathCount);
    
    REP(i,pathCount){
      int m = pathSize[i];
      value[i].resize(2 * m);
      fill(ALL(value[i]),0);
      int k=0;
      REP(j,m)value[i][j + m] = tree[k++].val;//init value
      for(int j=2*m-1;j>1;j-=2)
	value[i][j>>1] = ring.query(value[i][j],value[i][j^1]);
      
      delta[i].resize(2 * m);
      fill(ALL(delta[i]), (int)ring.mzero);
      
      len[i].resize(2*m);
      fill(ALL(len[i]),0);
      fill(len[i].begin()+m,len[i].begin()+2*m,1);
      for(int j=2*m-1;j>1;j-=2)len[i][j>>1]=len[i][j]+len[i][j^1];
    }
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

  void pushDelta(int pt, int i){
    int d=0;
    for(;(i>>d)>0;d++);
    DWN(dd,d-1,0){
      int x=i>>dd;
      value[pt][x>>1]=apply(pt,x>>1);
      delta[pt][x]=ring.modify(delta[pt][x],delta[pt][x>>1]);
      delta[pt][x^1]=ring.modify(delta[pt][x^1],delta[pt][x>>1]);
      delta[pt][x>>1]=ring.mzero;
    }
  }

  static int deltaSegment(int delta, int segmentLength){
    if(delta==ring.mzero)return ring.mzero;
    // Here you must write a fast equivalent of following slow code:
    // int result = delta;
    // for (int i = 1; i < segmentLength; i++) result = ring.query(result, delta);
    // return result;
    return delta;
  }
  //value with delta
  int apply(int pt,int i){
    return ring.modify(value[pt][i],deltaSegment(delta[pt][i],len[pt][i]));
  }
  
  int queryPath(int pt, int from, int to){
    int k=value[pt].size()>>1,res = ring.qzero;
    from+=k,to+=k,pushDelta(pt,from),pushDelta(pt,to);
    for(;from<=to;from=(from+1)>>1,to=(to-1)>>1){
      if(from & 1)res=ring.query(res,apply(pt,from));
      if(!(to & 1))res=ring.query(res,apply(pt,to));
    }
    return res;
  }
  
  void modifyPath(int pt, int from, int to, int del){
    int k=value[pt].size()>>1,ta=-1,tb=-1;
    from+=k,to+=k,pushDelta(pt,from),pushDelta(pt,to);
    for(;from<=to;from=(from+1)>>1, to=(to-1)>>1){
      if(from & 1)delta[pt][from]=ring.modify(delta[pt][from],del),ta=max(ta,from);
      if(!(to & 1))delta[pt][to]=ring.modify(delta[pt][to],del),tb=max(tb,to);
    }
    for(int i=ta;i>1;i>>=1) value[pt][i>>1]=ring.query(apply(pt,i),apply(pt,i^1));
    for(int i=tb;i>1;i>>=1) value[pt][i>>1]=ring.query(apply(pt,i),apply(pt,i^1));
  }
  
#define up(a,b) for(int r; !isAncestor(r = pathRoot[path[a]], b); a = tree[r].parent)
  
  int query(int a, int b) {
    int res = ring.qzero;
    up(a,b)res = ring.query(res, queryPath(path[a], 0, pathPos[a]));
    up(b,a)res = ring.query(res, queryPath(path[b], 0, pathPos[b]));
    return ring.query(res,queryPath(path[a], min(pathPos[a], pathPos[b]),max(pathPos[a], pathPos[b])));
  }

  void modify(int a, int b, int del){
    up(a,b)modifyPath(path[a],0,pathPos[a],del);
    up(b,a)modifyPath(path[b],0,pathPos[b],del);
    modifyPath(path[a],min(pathPos[a], pathPos[b]),max(pathPos[a], pathPos[b]),del);
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
