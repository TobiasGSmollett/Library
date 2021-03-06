#include<cmath>
#include<algorithm>
#include<vector>
#include<climits>
#include<cfloat>
#include<iostream>
#include<cassert>
#include<ctime>
#include<cstdlib>

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
    //return (p.x!=x)?x<p.x:y<p.y;
    return (p.x!=x) ? x<p.x : (p.y!=y)? y<p.y : z<p.z;
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
    return Point(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
  }
  static Point cross(const Point &a, const Point &b, const Point &c) {
    return (b-a).cross(c-b);
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

private:
  Plane(Real a,Real b,Real c,Real d):a(a),b(b),c(c),d(d){}

public:
  Plane(Point normal, Point p){
    Point n = normal.normalize();
    (*this) = Plane(n.x, n.y, n.z, -n.dot(p));
  }

  Plane(Point p0,Point p1,Point p2){
    (*this) = Plane(Point::cross(p0,p1,p2), p0);
  }

  Real dist(Point p) const {
    return abs(a*p.x + b*p.y + c*p.z + d);
  }

  //(-1,0,1) -> (back,on,front)
  int position(Point p) const {
    return sgn(a*p.x + b*p.y + c*p.z + d);
  }

  bool isIntersection(Segment seg){
    int a=position(seg.pos),b=position(seg.vec);
    return abs(a+b)<2;
  }
  
  Point intersection(Segment seg){
    assert(isIntersection(seg));
    Real l = dist(seg.pos);
    Real vec_l = seg.vec.dot(seg.vec);
    Point vec = seg.vec / vec_l;
    Real dt = vec.dot(Point(-a,-b,-c));
    Real pln_l=l/dt;
    return seg.pos + vec * pln_l;
  }

  bool isParallel(Plane p){
    Real dt = a*p.a + b*p.b + c*p.c;
    return sgn(dt, -1.0) <= 0 || 0 <= sgn(dt);
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

  bool isIntersection(Plane p){
    int side[3];
    for(int i=0;i<3;i++)side[i]=p.position(pos[i]);
    for(int i=0;i<2;i++)
      if(side[i] != side[i+1])return true;
    return false;
  }

  bool check(Triangle tri){
    Point v[3];
    bool hit[3];
    for(int i=0;i<3;i++)v[i]=pos[(i+1)%2]-tri.pos[i];
    for(int i=0;i<3;i++)
      hit[(i+1)%2]=tri.isIntersection(Segment(pos[i],v[(i+1)%2]));
    for(int i=0;i<3;i++)
      if(hit[i] && hit[(i+1)%2])return true;
    return false;
  }
  
  bool isIntersection(Triangle tri){
    return check(tri) || tri.check(*this);
  }
};


/**********************TEST*************************/

Real myrand(){
  return rand()%100;
}

Point randomPoint(){
  return Point(myrand(),myrand(),myrand());
}

Segment randomSegment(){
  return Segment(randomPoint(),randomPoint());
}

Plane randomPlane(){
  return Plane(randomPoint(),randomPoint());
  return Plane(randomPoint(),randomPoint(),randomPoint());
}

void testPlaneParallel(){
  for(int i=0;i<100000;i++){
    Plane a=randomPlane(),b=a;
    b.d+=myrand();
    assert(a.isParallel(b));
  }
}

void testPlaneSegmentIntersection(){
  for(int i=0;i<100;i++){
    Plane a = randomPlane();
    Segment b = randomSegment();
    if(a.isIntersection(b)){
      Point res = a.intersection(b);
      cout << res.x << " " << res.y << " " << res.z << endl;
    }
    //cout << a.a << " " << a.b << " " << a.c << " " << a.d << endl;
    //cout << b.pos.x << " " << b.pos.y << " " << b.pos.z << endl;
    //cout << b.vec.x << " " << b.vec.y << " " << b.vec.z << endl;
    //a.isIntersection2(b);
    //cout << a.isIntersection(b) <<" "<< a.isIntersection2(b) << endl;
    //assert(a.isIntersection(b) == a.isIntersection2(b));
  }

  
}

int main(void){
  srand(time(NULL));
  testPlaneParallel();
  testPlaneSegmentIntersection();

  return 0;
}
