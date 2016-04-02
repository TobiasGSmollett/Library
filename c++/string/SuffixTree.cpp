#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>

using namespace std;

const int SIZE = 128;

struct Node {
	int b,e,d; //begin, end, distance from root
	Node *parent, *children[SIZE], *suffixLink;

  Node():b(0),e(0),d(0),parent(NULL),suffixLink(NULL){
    fill(children, children + SIZE, (Node *)0);
  }
	Node(int b, int e, int d, Node *p):b(b),e(e),d(d),parent(p),suffixLink(NULL){
    fill(children, children + SIZE, (Node *)0);
	}
};

Node *buildSuffixTree(string s){
  int n = s.size(),a[n];
  for(int i = 0; i < n; i++)a[i] = (int)s[i];

  Node *root = new Node();
  Node *node = root;
  for(int i = 0, tail = 0; i < n; i++, tail++){
    Node *last = NULL;
    while(tail >= 0){
      Node *ch = node->children[a[i - tail]];
      while(ch != NULL && tail >= ch->e - ch->b){
        tail -= ch->e - ch->b;
        node = ch;
        ch = ch->children[a[i - tail]];
      }

      if(ch == NULL){
        node->children[a[i]] = new Node(i, n, node->d + node->e - node->b, node);
        if(last != NULL)last->suffixLink = node;
        last = NULL;
      } else {
        int afterTail = a[ch->b + tail];
        if(afterTail == a[i]){
          if(last != NULL)last->suffixLink = node;
          break;
        } else {
          Node *splitNode = new Node(ch->b, ch->b + tail, node->d + node->e - node->b, node);
          splitNode->children[a[i]] = new Node(i, n, ch->d + tail, splitNode);
          splitNode->children[afterTail] = ch;
          ch->b += tail;
          ch->d += tail;
          ch->parent = splitNode;
          node->children[a[i - tail]] = splitNode;
          if(last != NULL)last->suffixLink = splitNode;
          last = splitNode;
        }
      }
      if(node == root)tail--;
      else node = node->suffixLink;
    }
  }
  return root;
}

int lcsLength;
int lcsBeginIndex;
//longest common substring
int lcs_traverse(Node *node, int i1, int i2){
  if(node->b <= i1 && i1 < node->e)return 1;
  if(node->b <= i2 && i2 < node->e)return 2;

  int visited = 0;
  for(int f = 0; f < SIZE; f++)
    if(node->children[f] != NULL)
      visited |= lcs_traverse(node->children[f], i1, i2);

  if(visited == 3){
    int curLength = node->d + node->e - node->b;
    if(lcsLength < curLength){
      lcsLength = curLength;
      lcsBeginIndex = node->b - node->d;
    }
  }
  return visited;
}

string lcs(string s1, string s2){
  string s = s1 + '\1' + s2 + '\2';
  Node *tree = buildSuffixTree(s);

  lcsLength = 0;
  lcsBeginIndex = 0;
  lcs_traverse(tree, s1.size(), s1.size() + s2.size() + 1);
  return s.substr(lcsBeginIndex, lcsLength);
}

string longest_palindrome(string s1){
  string s2 = s1;
  reverse(s2.begin(),s2.end());
  return lcs(s1, s2);
}

vector<int> search(string s){

}

int main(void){
  string s1,s2;
  //cin >> s1 >> s2;
  //cout << lcs(s1,s2) << endl;
cin >> s1;
cout << longest_palindrome(s1) << endl;

  return 0;
}
