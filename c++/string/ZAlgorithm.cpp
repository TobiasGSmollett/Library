//See: http://codeforces.com/blog/entry/3107

#include<algorithm>
#include<iostream>
#include<vector>
#include<string>

using namespace std;

vector<int> Z(string s) {
  vector<int> z(s.size());
  int sz = z.size();
  for(int i=1,l=0,r=0;i<sz;i++){
    if(i<=r) z[i]=min(r-i+1, z[i-l]);
    while(i+z[i] < sz && s[z[i]] == s[i+z[i]])z[i]++;
    if(r < i+z[i]-1)l=i,r=i+z[i]-1;
  }
  return z;
}

vector<int> find(string s, string pattern) {
  vector<int> z = Z(pattern + "\1" + s);
  vector<int> res;
  int pl=pattern.size(),zl=z.size();
  for(int i = pl + 1; i < zl; i++)
    if(z[i] == pl)res.push_back(i - pl - 1);
  return res;
}

int main(void){

  return 0;
}
