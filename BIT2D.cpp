/*
  Unverified.
*/
#include<iostream>
#include<vector>
#include<algorithm>

#define max_n 100000
#define max_x 100000
#define max_y 100000

using namespace std;

typedef long long ll;

ll bit[2*max_n+10][2*max_n+10];
 
void add(int x,int y,ll val){
  int x1;
  while(y<=max_y){
    x1=x;
    while(x1 <= max_x){
      bit[y][x1]+=val;
      x1+=x1&-x1;
    }
    y+=y&-y;
  }
}

ll sum(int x,int y){
  ll s=0;
  int x1;
  while(y>0){
    x1=x;
    while(x1>0){
      s+=bit[y][x1];
      x1-=x1&-x1;
    }
    y-=y&-y;
  }
  return s;
}
 
