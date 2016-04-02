/*
  See: http://www37.atwiki.jp/uwicoder/pages/2842.html
*/

#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<queue>
#include<set>

using namespace std;

struct State {
  int len,link,firstPos,id;
  vector<int> invLinks,next;

  State():firstPos(-1),next(128,-1){}
};

vector<State> buildSuffixAutomaton(string s){
  int n=s.size();
  vector<State> st(max(2,2*n-1));
  st[0].link=-1;
  int last = 0, size = 1;
  
  for(int i=0;i<n;i++){
    char c=s[i];
    int cur=size++,p=last;
    st[cur].len=i+1,st[cur].firstPos=i;
    for(; p != -1 && st[p].next[c] == -1;p=st[p].link)st[p].next[c] = cur;
    if (p == -1)st[cur].link = 0;
    else {
      int q = st[p].next[c];
      if (st[p].len+1 == st[q].len)st[cur].link = q;
      else {
	int clone = size++;
	st[clone].len = st[p].len+1;
	st[clone].next = st[q].next;
	st[clone].link = st[q].link;
	for (; p != -1 && st[p].next[c] == q; p = st[p].link)st[p].next[c] = clone;
	st[q].link = st[cur].link = clone;
      }
    }
    last = cur;
  }
  for (int i=1;i<size;i++)st[st[i].link].invLinks.push_back(i);
  return st;
}

vector<int> findAllPos(vector<State> automaton, string pattern){
  int node=0, pl=pattern.size();
  for(int i=0;i<pl;i++){
    char c=pattern[i];
    int next = automaton[node].next[c];
    if(next == -1)return vector<int>();
    node = next;
  }

  vector<int> pos;
  queue<int> q;
  q.push(node);
  while (!q.empty()) {
    State cur = automaton[q.front()];q.pop();
    if(cur.firstPos != -1) pos.push_back(cur.firstPos-pl+1);
    for(int i=0;i<cur.invLinks.size();i++)q.push(cur.invLinks[i]);
  }
  sort(pos.begin(),pos.end());
  return pos;
}

string lcs(string a, string b) {
  vector<State>st = buildSuffixAutomaton(a);
  int len = 0, maxLen = 0, maxPos = -1, bl = b.size();

  for(int i=0,cur=0;i<bl;i++){
    char c = b[i];
    if(st[cur].next[c] == -1) {
      for(; cur != -1 && st[cur].next[c] == -1; cur = st[cur].link);
      if(cur == -1) {
	cur = len = 0;
	continue;
      }
      len=st[cur].len;
    }
    cur=st[cur].next[c];
    if(maxLen < ++len)maxLen=len,maxPos=i;
  }
  return b.substr(maxPos-maxLen+1, maxPos+1);
}

void dfs(vector<State> &automaton,int v,bool *vis,vector<int> &res){
  vis[v]=true;
  State cur=automaton[v];
  for(int i=0;i<128;i++)
    if(cur.next[i]!=-1 && !vis[cur.next[i]])dfs(automaton,cur.next[i],vis,res);

  res.push_back(v);
}

void topologicalSort(vector<State> &automaton){
  int n=automaton.size();
  bool vis[n];
  fill(vis,vis+n,false);
  vector<int>res;
  for(int i=0;i<n;i++)
    if(!vis[i])dfs(automaton,i,vis,res);
  
  reverse(res.begin(),res.end());
  for(int i=0;i<res.size();i++)automaton[res[i]].id=i;
}

bool cmp(const State &a, const State &b){
  return a.id < b.id;
}

//相異なるsubstringの個数
int numberOfDistinctSubstrings(vector<State> &automaton){
  int n=automaton.size(),dp[n],res=0;

  topologicalSort(automaton);
    
  fill(dp,dp+n,0);
  vector<State>nx=automaton;
  sort(nx.begin(), nx.end(),cmp);
  
  for(int i=0;i<n;i++){
    State cur=nx[i];
    res += dp[i], dp[i] = max(dp[i],1);
    for(int j=0;j<128;j++)
      if(cur.next[j]>=0 && automaton[cur.next[j]].id>i)
	dp[automaton[cur.next[j]].id]+=dp[i];
  }
  return res;
}


int main(void){
  
  string s,t;
  while(cin >> s){
    vector<State> res=buildSuffixAutomaton(s);
    
    set<string>S;
    for(int i=0;i<s.size();i++){
      for(int j=i+1;j<=s.size();j++){
	S.insert(s.substr(i,j-i));
      }
    }


    cout << numberOfDistinctSubstrings(res) << " " << S.size()<< endl;
  }
  return 0;
}
