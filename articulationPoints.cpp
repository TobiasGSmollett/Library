#include<iostream>
#include<vector>
#include<algorithm>

#define INF (1<<29)
#define MAX_V 10000

using namespace std;

vector<int>G[MAX_V];
int par[MAX_V],pnum[MAX_V],lowest[MAX_V],num;
bool used[MAX_V];
 
void dfs(int v,int prev){
  pnum[v]=num,lowest[v]=num;
  num++;
  used[v]=true;
 
  for(int i=0;i<G[v].size();i++){
    int nx=G[v][i];
    if(!used[nx]){
      par[nx]=v;
      dfs(nx,v);
      lowest[v]=min(lowest[v],lowest[nx]);
    }
    else if(nx!=prev){
      lowest[v]=min(lowest[v],pnum[nx]);
    }
  }
}
 
vector<int> ArticulationPoints(int n){
  fill(pnum,pnum+MAX_V,INF);
  fill(lowest,lowest+MAX_V,INF);
  fill(used,used+MAX_V,false);
 
  dfs(0,-1);
  num=1;
  int cnt=0;
  vector<int>ans;
  for(int i=0;i<n;i++){
    if(par[i]==0 && i!=0)cnt++;
    if(par[i]>0 && lowest[i]>=pnum[par[i]])ans.push_back(par[i]);
  }
 
  if(cnt>=2)ans.push_back(0);
  sort(ans.begin(),ans.end());
  ans.erase(unique(ans.begin(),ans.end()),ans.end());
  return ans;
}
