/*
Verified. AOJ1163
Hopcroft Karp
O(E sqrt(V))
*/
#include <algorithm>
#include <iostream>
#include<vector>

#define MAX_L 50000
#define MAX_R 50000
#define MAX_E 150000

using namespace std;

int n1,n2;//（左、右）の頂点数
int edges;//辺の本数
int last[MAX_L];
int preve[MAX_E];
int head[MAX_E];
int match[MAX_R],dist[MAX_L],que[MAX_L];

void init(int _n1, int _n2){
  n1=_n1,n2=_n2,edges=0;
  fill(last,last+n1,-1);
}

void add_edge(int u, int v){
  head[edges]=v;
  preve[edges]=last[u];
  last[u]=edges++;
}

bool used[MAX_L];

void bfs(){
  fill(dist,dist+n1,-1);
  int sizeQ=0;
  for (int u=0;u<n1;u++){
    if (!used[u]){
      que[sizeQ++]=u;
      dist[u]=0;
    }
  }
  for (int i=0;i<sizeQ;i++){
    int u1=que[i];
    for (int e=last[u1];e>=0;e=preve[e]){
      int u2=match[head[e]];
      if (u2>=0 && dist[u2]<0){
	dist[u2]=dist[u1]+1;
	que[sizeQ++]=u2;
      }
    }
  }
}

bool vis[MAX_L];

bool dfs(int u1) {
  vis[u1]=true;
  for (int e=last[u1];e>=0;e=preve[e]){
    int v=head[e],u2=match[v];
    if (u2<0 || (!vis[u2] && dist[u2]==dist[u1]+1 && dfs(u2))){
      match[v]=u1;
      return used[u1]=true;
    }
  }
  return false;
}

int bipartite_matching(){
  fill(used,used+n1,false);
  fill(match,match+n2,-1);
  int flow=0;
  for (;;){
    bfs();
    fill(vis,vis+n1,false);
    int f=0;
    for(int u=0;u<n1;u++)
      if(!used[u] && dfs(u))f++;
    if (!f)return flow;
    flow+=f;
  }
}

/*
//AOJ 1163
int gcd(int a,int b){
    if(b==0)return a;
    return gcd(b,a%b);
}

int main(void){
  
  int m,n;
  
  while(cin >> m >> n,m|n){
 
    init(m,n);
    
    vector<int>b(m),r(n);
    for(int i=0;i<m;i++)cin >> b[i];
    for(int i=0;i<n;i++)cin >> r[i];
    
    for(int i=0;i<m;i++){
      for(int j=0;j<n;j++){
	if(gcd(b[i],r[j])!=1)add_edge(i,j);
      }
    }

    cout << bipartite_matching() << endl;
  }
  
  return 0;
}
*/
