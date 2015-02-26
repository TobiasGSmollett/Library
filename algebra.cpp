#include<iostream>
#include<cmath>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;

typedef long long ll;


int gcd(int a,int b){
  return b?gcd(b,a%b):a;
}

int lcm(int a,int b){
  return a/gcd(a,b)*b;
}

int mod(int x,int m){
  return (m+x%m)%m;
}

//mod inverse
int inv(int a,int m){
  int x,y;
  extgcd(a,m,x,y);
  return mod(x,m);
}

/*
O(log n)
*/
ll mod_pow(ll x, ll n, ll mod){
  if(n==0)return 1;
  ll res=mod_pow(x*x%mod,n/2,mod);
  if(n&1)res=res*x%mod;
  return res;
}

/*
Verified. AOJ NTL_1_E
gcd(a,b)=a*x+b*y
*/
void extgcd(int a,int b,int &x,int &y){
  x=1,y=0;
  if(b!=0)extgcd(b,a% b,y,x),y-=(a/b)*x; 
}

/*
//return {gcd(a,b),x,y}

typedef pair<int,int> pii;
typedef pair<int,pii> piii;

piii extgcd(int a,int b){
  int x=1,y=0,x1=0,y1=1,t;
  while(b!=0){
    int q=a/b;
    t=x,x=x1,x1=t-q*x1;
    t=y,y=y1,y1=t-q*y1;
    t=b,b=a-q*b,a=t;
  }
  return a>0?piii(a,pii(x,y)):piii(-a,pii(-x,-y));
}
*/

/*
Verified. AOJ 2353
o=1,2,3,4(+,-,*,/)
*/
ll m1=999979,m2=999983;

ll calc(ll x,int o,ll y){
  ll a=0,b=0,p,q;
 
  if(o==1)a=x+y,b=x+y;
  if(o==2)a=x-y,b=x-y;
  if(o==3)a=x*y,b=x*y;
  if(o==4)a=x*mod_inv(y,m1),b=x*mod_inv(y,m2);

  a=mod(a,m1),b=mod(b,m2);
  extgcd(m1,m2,p,q);
   
  return (b*m1*p+a*m2*q)%(m1*m2);
}

int main(void){}
