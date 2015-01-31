#include<cmath>
#include<algorithm>
#include<iostream>
#include<vector>
#include<climits>
#include<cfloat>
#define curr(P, i) P[(i) % P.size()]
#define next(P, i) P[(i+1) % P.size()]
#define prev(P, i) P[(i+P.size()-1) % P.size()]


using namespace std;

typedef double Real;

Real EPS = 1e-8;
const Real PI = acos(-1);

int sgn(Real a, Real b=0){return a<b-EPS?-1:a>b+EPS?1:0;}
Real sqr(Real a){return sqrt(max(a,(Real)0));}

struct Point{  
  Real add(Real a, Real b){
    if(abs(a+b) < EPS*(abs(a)+abs(b)))return 0;
    return a+b;
  }

  Real x, y;
  Point(){}
  Point(Real x,Real y) : x(x) , y(y){}

  Point operator + (Point p){return Point(add(x,p.x), add(y,p.y));}
  Point operator - (Point p){return Point(add(x,-p.x), add(y,-p.y));}
  Point operator * (Real d){return Point(x*d,y*d);}
  Point operator / (Real d){return Point(x/d,y/d);}
  bool operator == (Point p){return !sgn(dist(p));}
  //昇順
  bool operator < (Point p)const{return (p.x!=x)?x<p.x:y<p.y;}
  Real norm(){return sqr(x*x+y*y);}
  Real dist(Point a){return (*this-a).norm();}
  Real dot(Point a){return x*a.x+y*a.y;}
  Real cross(Point a){return x*a.y-y*a.x;}
  //点pを中心に角度r(radius)だけ半時計回りに回転する
  Point rotate(Real r,Point p = Point(0,0)){
    Real ta=cos(r)*(x-p.x)-sin(r)*(y-p.y)+p.x;
    Real tb=sin(r)*(x-p.x)+cos(r)*(y-p.y)+p.y;
    return Point(ta,tb);
  }
  Real arg(){
    if(sgn(x)>0)return atan(y/x);
    if(sgn(x)<0)return atan(y/x)+PI;
    if(sgn(y)>0)return PI/2;
    if(sgn(y)<0)return 3*PI/2;
    return 0;
  }
};

//a -> b -> c
int ccw(Point a, Point b, Point c) {
  b = b-a; c = c-a;
  if (b.cross(c) > 0)   return +1;       // counter clockwise
  if (b.cross(c) < 0)   return -1;       // clockwise
  if (b.dot(c) < 0)     return +2;       // c--a--b on line
  if (b.norm() < c.norm()) return -2;    // a--b--c on line
  return 0;                              // a--c--b on line
}

/*
  Unverified.
  原点を始点とするベクトルa,b,c
  cがa -> bに反時計回りで回る角の
  内部,辺上,外部(1,0,-1)
  のどこにあるか
*/
int visible(Point a, Point b, Point c){
  return sgn(sgn(a.cross(c))-sgn(b.cross(c))-sgn(a.cross(b)));
}

struct Line{
  Point a,b;

  Line(){}
  Line(Point a,Point b):a(a),b(b){}

  bool on(Point c){return abs(ccw(a,b,c))!=1;}
  Real dist(Point c){return abs((b-a).cross(c-a))/(b-a).norm();}
  bool isOrthogonal(Line l){return (a-b).dot(l.a-l.b)==0.0;}
  bool isParallel(Line l){return (a-b).cross(l.a-l.b)==0.0;}
  bool isIntersection(Line l){return !((a-b).cross(l.a-l.b)==0.0);}
  //平行でない前提
  Point intersectionPoint(Line l){
    return a+(b-a)*((l.b-l.a).cross(l.a-a)/(l.b-l.a).cross(b-a));
  }

  //射影
  Point project(Point p){
    Point base=b-a;
    Real t=(p-a).dot(base)/base.dot(base);
    return a+base*t;
  }

  /*
    Verified. AOJ 0081
    p1,p2を通る直線を対称軸として点Qと線対称の座標R
  */
  Point symmetry(Point Q){
    return Q+(project(Q)-Q)*2;
  }

  /*
    Unverified.
    直線を距離hだけ左側に平行移動した直線
  */
  Line parallelShift(Real h){
    Real t = atan((a.y-b.y)/(a.x-b.x));
    Point a_(a.x + h*cos(t+PI/2), a.y + h*sin(t+PI/2));
    Point b_(b.x + h*cos(t+PI/2), b.y + h*sin(t+PI/2));
    if(ccw(a,b,a_)==-1)a_=symmetry(a_),b_=symmetry(b_);
    return Line(a_,b_);
  }
};

struct Segment: public Line{
  
  Segment(){}
  Segment(Point a,Point b):Line(a,b){}

  bool on(Point c){return ccw(a,b,c)==0;}
  bool on(Line l){return l.on(a)&&l.on(b);}
  bool overlap(Segment s){
    return isParallel(s) && (on(s.a)||on(s.b)||s.on(a)||s.on(b));
  }
  bool isParallel(Segment s){return (a-b).cross(s.a-s.b)==0.0;}

  Real dist(Point c){
    if((b-a).dot(c-a)<EPS)return c.dist(a);
    if((a-b).dot(c-b)<EPS)return c.dist(b);
    return abs((b-a).cross(c-a))/b.dist(a);    
  }

  //Unverified.
  bool isIntersection(Line l){
    return (ccw(l.a,l.b,a)*ccw(l.a,l.b,b)<0);
  }

  /*
    Verified. AOJ CGL_2/B
  */
  bool isIntersection(Segment s){
    return (ccw(a,b,s.a)*ccw(a,b,s.b)<=0 &&
	    ccw(s.a,s.b,a)*ccw(s.a,s.b,b)<=0);
  }
  
  /*
    Verified. AOJ CGL_2/C
  */
  Point intersectionPoint(Segment s){
    Point q=s.b-s.a;
    Real d1=abs(q.cross(a-s.a));
    Real d2=abs(q.cross(b-s.a));
    Real t=d1/(d1+d2);
    return a+(b-a)*t;
  }
};

struct Circle{
  Point p;
  Real r;
  Circle(){}
  Circle(Point p, Real r) : p(p) , r(r){}
  
  /*
    Verified. AOJ CGL_7/D 
    直線と円の交点。
  */
  vector<Point> intersectionPoints(Line l){
    if(sgn(l.dist(p),r)>0)return vector<Point>();
    Point q=l.project(p);
    Real t=(p-q).norm(),d=sqr(r*r-t*t);
    Point base=l.a-l.b;
    vector<Point>res;
    res.push_back(q+base*(d/base.norm()));
    res.push_back(q-base*(d/base.norm()));
    return res;
  }
  
  /*
    Verified. AOJ CGL_7/E
    2円の交点を求める
  */
  vector<Point> intersectionPoints(Circle b){
    vector<Point> res;
    Point tmp=p-b.p;
    if(tmp.norm()>pow(r+b.r,2))return res;
    
    Real L=p.dist(b.p);
    Real C=atan2(b.p.y-p.y,b.p.x-p.x);
    Real a=acos((L*L+r*r-b.r*b.r)/(2*L*r));
    
    res.push_back(Point(p.x+r*cos(C+a), p.y+r*sin(C+a)));
    res.push_back(Point(p.x+r*cos(C-a), p.y+r*sin(C-a)));
    return res;
  }
  
  /*
    Verified. AOJ 0010
    3点p,q,rを通る円を求める
  */
  Circle circumscribedCircle(Point p, Point q, Point r){
    Point a=(q-p)*2,b=(r-p)*2;
    Point c(p.dot(p)-q.dot(q),p.dot(p)-r.dot(r));
    Circle res;
    res.p.x=a.y*c.y-b.y*c.x;
    res.p.y=b.x*c.x-a.x*c.y;
    res.p=res.p/a.cross(b);
    return circle(res.p, p.dist(res.p));
  }
  
  /*
    Verified. AOJ 2201
    共通内,外接線
  */
  vector<Line> commonTangent(Circle x){
    vector<Line>res;
    x.p=x.p-p;
    Real a=x.p.arg();
    Real b[2]={(r-x.r),(r+x.r)};
    for(int i=0;i<2;i++){
      b[i]/=x.p.norm();
      if(sgn(b[i],-1.0)<0 || sgn(b[i],1.0)>0)return res;
      Real c=asin(b[i])-asin(1.0);
      res.push_back(tangent(a-c));
      res.push_back(tangent(a+c));
    }
    return res;
  }
  
  Line tangent(Real theta){
    Point a(r*cos(theta),r*sin(theta));
    Point b(a.y,-a.x);
    return Line(p+a,p+a+b);
  }
};

struct Polygon{
  //頂点。反時計回りに順番で与えられていること。
  vector<Point>v;
  
  Polygon(){}
  Polygon(int n){v.resize(n);}
  Polygon(vector<Point>v):v(v){} 
  
  /*
    Verified. AOJ CGL_3/B
    凸性判定
  */
  bool isConvex() {
    for (int i=0; i<v.size();i++){
      if (ccw(prev(v,i),curr(v,i),next(v,i))==-1)return false;
    }
    return true;
  }
  /*
    Verified. AOJ 0012
    多角形が凸である必要はない。
    
    点が多角形に含まれれば1,
    点が辺または頂点の上にあれば2
    それ以外は0を返す。
  */
  int contain(Point p){
    bool in=false;
    for(int i=0;i<v.size();i++){
      Point a=curr(v,i)-p,b=next(v,i)-p;
      if(a.y>b.y)swap(a,b);
      if(a.y<=0 && 0<b.y && a.cross(b)<0)in=!in;
      if(a.cross(b)==0 && a.dot(b)<=0)return 2;
    }
    return in?1:0;
  }
  
  /*
    Verified. AOJ CGL_3/A
    多角形の面積計算。
    多角形の頂点が時計回りまたは反時計回り
  */
  Real area(){
    Real area=0;
    for(int i=0;i<v.size();i++)
      area+=(curr(v,i).x-next(v,i).x)*(curr(v,i).y+next(v,i).y);
    return abs(area)/2.0;
  }
  
  /*
    Verified. AOJ CGL_4/C
    凸多角形の切断
    凸多角形を直線lで切断し
    左側の多角形を返す。
  */
  Polygon convexCut(Line l){
    vector<Point>res;
    for(int i=0;i<v.size();i++){
      Point a=curr(v,i),b=next(v,i),p=l.b-l.a;
      if(ccw(l.a,l.b,a)!=-1)res.push_back(a);
      if(sgn(p.cross(a-l.a)*p.cross(b-l.a))<0)
	res.push_back(Segment(a,b).intersectionPoint(l));
    }
    return res;
  }
  
  /*
    Verified. AOJ CGL_4/A
    凸包
   */
  vector<Point>convexHull(){
    vector<Point>v1=v;
    sort(all(v1));
    int k=0;
    vector<Point> qs(v1.size()*2);
    
    for(int i=0;i<v1.size();i++){
      while(k>1 && (qs[k-1]-qs[k-2]).cross(v1[i]-qs[k-1])<0)k--;
      qs[k++]=v1[i];
    }
    
    for(int i=v1.size()-2,t=k;i>=0;i--){
      while(k>t && (qs[k-1]-qs[k-2]).cross(v1[i]-qs[k-1])<0)k--;
      qs[k++]=v1[i];
    }
    qs.resize(k-1);
    return qs;
  }
};



