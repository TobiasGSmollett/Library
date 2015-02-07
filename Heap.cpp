#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdio>
#include<queue> 

using namespace std;

/*
  Unverified.
*/

template<class T>
struct Heap{

private:
  priority_queue<T>maxHeap,delMax;
  priority_queue<T, vector<T>, greater<T> >minHeap,delMin;

  void update(bool isMax){
    while(isMax && !maxHeap.empty() && !delMax.empty() &&
	  maxHeap.top()==delMax.top())
      maxHeap.pop(),delMax.pop();
   
    while(!isMax && !minHeap.empty() && !delMin.empty() &&
	  minHeap.top()==delMin.top())
      minHeap.pop(),delMin.pop();
  }

public:

  T min(){ update(false); return minHeap.top(); }
  T max(){ update(true); return maxHeap.top(); }

  void push(T x){
    maxHeap.push(x);
    minHeap.push(x);
  }

  void minpop(){
    delMax.push(min());
    minHeap.pop();
    update(false);
  }

  void  maxpop(){
    delMin.push(max());
    maxHeap.pop();
    update(true);
  }
};
 
/*
int main(void){

  Heap<int>q;

  string s;
  while(cin >> s){
    if(s=="push"){
      int a;
      cin >> a;
      q.push(a);
    }
    if(s=="max")cout << q.max() << endl;
    if(s=="min")cout << q.min() << endl;
    if(s=="maxpop")q.maxpop();
    if(s=="minpop")q.minpop();
  }
  return 0;
}
*/
