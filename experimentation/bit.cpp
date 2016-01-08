#include<iostream>
#include<vector>
#include<algorithm>

#define all(c) (c).begin(),(c).end()

using namespace std;

class Bit{
  typedef unsigned long long ull;
  vector<ull>bit;

public:

  const int size;

  Bit(int n):bit(n/64),size(n){
    fill(all(bit),0);
  }

  Bit(vector<ull> b):bit(b),size(b.size()){}

  void clear(){ fill(all(bit),0); }

  Bit operator << (int n) const {
    int m = size >> 6;
    vector<ull> res = bit;
    for(int i=m-1; i>=0; i--){
      res[i+1] |= (res[i] >> (64-n));
      res[i] <<= n;
    }
    return Bit(res);
  }
  
  Bit operator >> (int n) const {
    int m = size >> 6;
    vector<ull> res = bit;
    for(int i=0; i<m; i++){
      res[i] |= (res[i+1] << (64-n));
      res[i] >>= n;
    }
    return res;
  }
  
  Bit operator & (const Bit &that) const {}
  Bit operator | (const Bit &that) const {}
  Bit operator ^ (const Bit &that) const {}
  Bit operator ! () const {}

  //(bit[id/64] >> (id%64)) & 1
  bool operator [](int id) const {
    return (bit[id >> 6] >> (id & 63)) & 1;
  }
  
  bool operator == (const Bit &that) const {
    bool res = true;
    int n = size >> 6;
    for(int i=0; i < n; i++) res &= bit[i]==that[i];
    return res;
  }
  
  bool operator != (const Bit &that) const {
    return !(*this == that);
  }

  void operator <<= (int n){}
  void operator >>= (int n){}
  void operator &= (const Bit &that){}
  void operator |= (const Bit &that){}
  void operator ^= (const Bit &that){}

  bool contains(Bit that) const {}
};

int main(void){

  cout << Bit(100).size << endl;

  
  return 0;
}
