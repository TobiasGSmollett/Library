#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>

#define all(c) (c).begin(),(c).end()

using namespace std;

class Bit{
  typedef unsigned long long ull;
  vector<ull>bit;

  //bucketSize must be 2^n
  static const int bucketSize = 64;
  
public:
  
  const int size;

  Bit(int n):bit(n / bucketSize),size(n){
    fill(all(bit),0);
  }

  Bit(vector<ull> b):bit(b),size(b.size()){}

  void clear(){ fill(all(bit),0); }

  Bit operator << (int n) const {
    int m = size / bucketSize;
    int shiftBucket = n / bucketSize;
    n %= bucketSize;
    vector<ull> res = bit;
    for(int i=m-1; i>=0; i--){
      res[i + shiftBucket] |= (res[i] >> (bucketSize - n));
      res[i] <<= n;
    }
    return Bit(res);
  }
  
  Bit operator >> (int n) const {
    int m = size >> 6;
    vector<ull> res = bit;
    for(int i=0; i<m; i++){
      res[i] >>= n;
      res[i] |= (res[i+1] << (bucketSize - n));
    }
    return res;
  }
  
  Bit operator & (const Bit &that) const {
    vector<ull>a = (size > that.size ? bit : that.bit);
    vector<ull>b = (size < that.size ? bit : that.bit);
    int asize = a.size(), bsize = b.size();
    for(int i = 0; i < asize; i++)
      a[i] &= (i < bsize ? b[i] : 0);
    return a;
  }
  
  Bit operator | (const Bit &that) const {
    vector<ull>a = (size > that.size ? bit : that.bit);
    vector<ull>b = (size < that.size ? bit : that.bit);
    int asize = a.size(), bsize = b.size();
    for(int i = 0; i < asize; i++)
      a[i] |= (i < bsize ? b[i] : 0);
    return a;
  }
  
  Bit operator ^ (const Bit &that) const {
    vector<ull>a = (size > that.size ? bit : that.bit);
    vector<ull>b = (size < that.size ? bit : that.bit);
    int asize = a.size(), bsize = b.size();
    for(int i = 0; i < asize; i++)
      a[i] ^= (i < bsize ? b[i] : 0);
    return a;
  }
  
  Bit operator ! () const {}

  //(bit[id/64] >> (id%64)) & 1
  bool operator [](int id) const {
    return (bit[id >> 6] >> (id & (bucketSize - 1))) & 1;
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
  __int128 a;
  __uint128_t b;
  cout << Bit(100).size << endl;
  
  return 0;
}
