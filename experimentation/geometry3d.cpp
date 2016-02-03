#include<cmath>
#include<algorithm>
#include<vector>
#include<climits>
#include<cfloat>
#include<iostream>
#include<cassert>

#define curr(P, i) P[(i) % P.size()]
#define next(P, i) P[(i+1) % P.size()]
#define prev(P, i) P[(i+P.size()-1) % P.size()]

#define ALL(A) A.begin(), A.end()

using namespace std;

typedef double Real;

const Real EPS = 1e-8;
const Real PI = acos(-1);

int sgn(const Real &a, const Real &b=0){
  return a<b-EPS?-1:a>b+EPS?1:0;
}
Real sqr(const Real &a){
  return sqrt(max(a,(Real)0));
}

struct Point{  
  Real add(Real a, Real b) const {
    if(abs(a+b) < EPS*(abs(a)+abs(b)))return 0;
    return a+b;
  }

  Real x, y, z;
  Point(){}
  Point(Real x,Real y, Real z) : x(x) , y(y), z(z){}

  Point operator + (const Point &p) const {
    return Point(add(x,p.x), add(y,p.y), add(z,p.z));
  }
  Point operator - (const Point &p) const {
    return Point(add(x,-p.x), add(y,-p.y), add(z,-p.z));
  }
  Point operator * (const Real &d) const {
    return Point(x*d,y*d,z*d);
  }
  Point operator / (const Real &d) const {
    return Point(x/d,y/d,z/d);
  }
  bool operator == (const Point &p) const {
    return !sgn(dist(p));
  }
  bool operator < (const Point &p) const {
    return (p.x!=x)?x<p.x:y<p.y;
  }
  Real norm() const {
    return sqr(dot(*this)); //return sqr(x*x + y*y + z*z);
  }
  Real dist(const Point &a) const {
    return (*this-a).norm();
  }
  Real dot(const Point &a) const {
    return x*a.x+y*a.y+z*a.z;
  }
  Point cross(const Point &a) const {
    return Point(y*a.z-z*a.y,z*a.x-x*a.z,x*a.y-y*a.x);
  }
  static Point cross(const Point &a, const Point &b, const Point &c) {
    return (c-b).cross(b-a);
  }
  Point normalize() const {
    return (*this)/norm();
  }
};

struct Segment{
  Point pos,vec;
  Segment(Point p, Point v):pos(p),vec(v){}
};

struct Plane{
  Real a,b,c,d; //ax+by+cx+d=0

  Plane(Real a,Real b,Real c,Real d):a(a),b(b),c(c),d(d){}
  
  Plane(Point normal, Point p){
    Point n = normal.normalize();
    (*this) = Plane(n.x, n.y, n.z, -n.dot(p));
  }

  Plane(Point p0,Point p1,Point p2){
    (*this) = Plane(Point::cross(p0,p1,p2), p0);
  }

  Real dist(Point p) const {
    return a*p.x + b*p.y + c*p.z + d;
  }

  //(-1,0,1) -> (back,on,front)
  int position(Point p) const {
    return sgn(dist(p));
  }

  bool isIntersection(Segment seg){
    Real l = dist(seg.pos);
    Real vec_l = seg.vec.norm();
    if(sgn(vec_l,l)<0)return false;
    Point vec = seg.vec / vec_l;
    Real dt = vec.dot(Point(-a,-b,-c));
    Real pln_l=l/dt;
    if(sgn(pln_l,vec_l)>0)return false;
    return true;
  }

  bool isIntersection2(Segment seg){
    return position(seg.pos)!=position(seg.vec);
  }
  
  Point intersection(Segment seg){
    assert(isIntersection(seg));
    Real l = dist(seg.pos);
    Real vec_l = seg.vec.norm();
    Point vec = seg.vec / vec_l;
    Real dt = vec.dot(Point(-a,-b,-c));
    Real pln_l=l/dt;
    return seg.pos + vec * pln_l;
  }

  bool isParallel(Plane p){
    Real dt = a*p.a + b*p.b + c*p.c;
    return sgn(abs(dt),1.0) >= 0;
  }

  Segment intersection(Plane p){
    assert(!isParallel(p));
    Point vec = Point(a,b,c).cross(Point(p.a,p.b,p.c));
    Point pos;
    if(sgn(vec.z)!=0)pos=Point(b*p.d-p.b*d, p.a*d-a*p.d, 0.0)/vec.z;
    else if(sgn(vec.y)!=0)pos=Point(p.c*d-c*p.d, 0.0, a*p.d-p.a*d)/vec.y;
    else pos=Point(0.0, c*p.d-p.c*d, p.b*d-b*p.d)/vec.x;
    return Segment(pos, vec);
  }
};

struct Sphere{
  Point c;
  Real r;
  Sphere(Point c, Real r):c(c),r(r){}

  vector<Point> intersectionPoints(Segment seg){
    Point tmp = seg.pos-(*this).c;
    Real a = seg.vec.dot(seg.vec);
    Real b = seg.vec.dot(tmp);
    Real d = sqr(b*b-4.0*a*(tmp.dot(tmp))-r*r);
    if(sgn(d)<0)return vector<Point>();
    Real t0 = (d-b)/(2.0*a), t1 = (-d-b)/(2.0*a), t = 2.0;
    if(t > 1.0+EPS)return vector<Point>();
    vector<Point>res;
    if(-EPS < t0 && t0 < 1.0-EPS && t0 < t+EPS)
      res.push_back(seg.pos+seg.vec*t0);
    if(-EPS < t1 && t1 < 1.0-EPS && t1 < t+EPS)
      res.push_back(seg.pos+seg.vec*t1);
    return res;
  }
};

struct Triangle{
  Point pos[3];
  Plane plane;
  
  Triangle(Point p0, Point p1, Point p2):plane(Plane(p0,p1,p2)){
    pos[0]=p0,pos[1]=p1,pos[2]=p2;
  }

  bool contains(Point p){
    Point vt[3],v[3],c[3];
    for(int i=0;i<3;i++)vt[i]=pos[i]-p;
    for(int i=0;i<3;i++)v[i]=pos[(i+1)%3]-pos[i];
    for(int i=0;i<3;i++)c[i]=vt[i].cross(v[i]);
    Real dot0=c[0].dot(c[1]),dot1=c[1].dot(c[2]);
    return sgn(dot0 * dot1)>0;
  }

  bool isIntersection(Segment seg){
    if(!plane.isIntersection(seg))return false;
    Point tmp = plane.intersection(seg);
    return contains(tmp);
  }

  Point intersection(Segment seg){
    assert(isIntersection(seg));
    return plane.intersection(seg);
  }
};

int main(void){


  Point n=Point(1,2,3),p=Point(1,1,1);
  Plane d = Plane(n,p);
  cout << d.a << " " << d.b << " " << d.c << " " << d.d << endl;

  return 0;
}
