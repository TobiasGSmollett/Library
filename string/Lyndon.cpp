#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

string lyndonWord(string s) {
  s+=s;
  int n=s.size(),res=0;
  for(int i=0;i<n/2;){
    res=i;
    int j=i+1,k=i;
    while(j < n && s[k] <= s[j])(s[k]<s[j] ? k=i : k++),j++;
    while(i <= k)i+=j-k;
  }
  return s.substr(res,n/2);
}

vector<string> decompose(string s) {
  vector<string> res;
  int n=s.size(),i=0;
  while(i < n){
    int j=i+1,k=i;
    while(j < n && s[k] <= s[j])(s[k]<s[j] ? k=i : k++),j++;
    while(i <= k)res.push_back(s.substr(i, j-k)),i+=j-k;
  }
  return res;
}

int main(void){
  string s;
  while(cin >> s){
    vector<string>res=decompose(s);
    for(int i=0;i<res.size();i++)cout << res[i] << " ";
    cout << endl;
    cout << lyndonWord(s) << endl;
  }
  
  return 0;
}
