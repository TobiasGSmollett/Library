#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
#include<cassert>

#define INF (1<<29)
#define all(c) (c).begin(),(c).end()
#define ALL(a,b,c) all(a),all(b),std::inserter((c),c.begin())
#define each(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)

using namespace std;

int n,m;
vector<int>g[401];
set<int>Result;
// See : http://www.sciencedirect.com/science/article/pii/S0304397506003586
void tomita(set<int> R, set<int>P, set<int> X){

  if(P.empty() && X.empty()){
    if(Result.size()<R.size())Result=R;
    return;
  }
  
  set<int> tmp,Nu;

  set_union(ALL(P,X,tmp));
  int pivot = -1, mxdig = -1;
  each(it,tmp)
    if(mxdig < (int)g[*it].size())
      pivot = *it, mxdig = g[*it].size();

  for(int i=0;i<(int)g[pivot].size();i++)Nu.insert(g[pivot][i]);

  set<int>diff;
  set_difference(ALL(P,Nu,diff));

  each(v,diff){

    set<int>Nv,V;
    V.insert(*v);
    for(int i=0;i<(int)g[*v].size();i++)Nv.insert(g[*v][i]);

    set<int>a=R,b,c;
    a.insert(*v);
    set_intersection(ALL(P,Nv,b));
    set_intersection(ALL(X,Nv,c));

    tomita(a,b,c);
    
    set_difference(ALL(P,V,P));
    X.insert(*v);
  }
}

void Tomita(){
  Result.clear();
  set<int>R,P,X;
  for(int i=0;i<n;i++)P.insert(i);
  tomita(R,P,X);
}

//POJ 3692
int main(void){

  int G,B,tc=1;
  while(cin >> G >> B >> m,G|B|m){
    n=G+B;
    for(int i=0;i<n;i++)g[i].clear();

    for(int i=0;i<G;i++){
      for(int j=i+1;j<G;j++){
	g[i].push_back(j);
	g[j].push_back(i);
      }
    }
    
    for(int i=G;i<n;i++){
      for(int j=i+1;j<n;j++){
	g[i].push_back(j);
	g[j].push_back(i);
      }
    }
    
    for(int i=0;i<m;i++){
      int a,b;
      cin >> a >> b;
      a--,b--;
      g[a].push_back(G+b);
      g[G+b].push_back(a);
    }

    Tomita();
    cout << "Case "<<  tc++ <<":" << Result.size() << endl;
  }
  
  return 0;
}
