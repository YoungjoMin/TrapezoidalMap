#ifndef __TRAPEZOIDAL_MAP_H__
#define __TRAPEZOIDAL_MAP_H__

#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>


/*
Assumptions.
0. two point a,b a!=b then their x,y coordinates differ more than eps
1. no vertical line
2. two line intersects except line intersects at the endpoint of both line
3. a point cannot lies on a line except endpoint of the line.
4. all lines and points are inside the bounding box
*/
constexpr double eps = 1e-6; // |x-y|<eps then consider as same point

struct Point;
struct Line;
struct TNode;
struct Trapezoid;

struct Point {
	double x, y;
	Point() : x(0.0), y(0.0) {};
	Point(double x, double y) : x(x), y(y) {};
	bool isLeft(const Point& p) const; //this point is lefter than p
	bool isSame(const Point& p) const;

	friend Point operator-(const Point& p1, const Point& p2);
	friend Point operator+(const Point& p1, const Point& p2);
	Point operator*(double s);
	Point normalize();
	friend Point lefterPoint(const Point& p1, const Point& p2);
	friend std::ostream& operator<<(std::ostream& o, const Point& t);
};

struct Line {
	Point pl, pr; //pl is at left of p2
	Line(const Point& pl, const Point& pr);
	Line(const Line& l) : pl(l.pl), pr(l.pr) {};
	friend std::ostream& operator<<(std::ostream& o, const Line& l);
	bool isUpper(const Point& p) const;//this line is upper than p
};

struct Trapezoid {
	Line top, bottom;
	Point leftp, rightp;
	TNode* node;

	Trapezoid* upperright, * lowerright;
	Trapezoid* upperleft, * lowerleft;

	Trapezoid(Line top, Line bottom, Point leftp, Point rightp) : top(top), bottom(bottom), leftp(leftp), rightp(rightp) {}
	friend std::ostream& operator<<(std::ostream& o, const Trapezoid& t);
	bool isInside(const Point& pt);
	void updateLeftTrapezoid(Trapezoid* prv, Trapezoid* cur);
	void updateRightTrapezoid(Trapezoid* prv, Trapezoid* cur);
};

struct TNode {
	TNode* lc, * rc;
	virtual bool isLeaf() = 0;
	virtual TNode* query(const Point& pt) = 0;
};

struct XNode : TNode{
	Point p;
	XNode(Point p) : p(p) {}
	virtual bool isLeaf();
	virtual TNode* query(const Point& pt);
};

struct YNode : TNode {
	Line l;
	YNode(Line l) : l(l) {}
	virtual bool isLeaf();
	virtual TNode* query(const Point& pt);
};

struct LeafNode : TNode {
	std::vector<TNode **> parents; //at most 4 parents (from top,bottom, leftp, rightp)
	Trapezoid* t;

	LeafNode(Trapezoid* t);
	virtual bool isLeaf();
	virtual TNode* query(const Point& pt);
};

struct TrapezoidalMap {
	TNode* root;


	TrapezoidalMap(const Point& bottomLeft, const Point& topRight);
	Trapezoid* query(const Point& p);
	void insert(const Line& l);
	~TrapezoidalMap() = default;

private:
	void insert_two_segment_endpoint(Trapezoid* trapezoid, const Line& l);
	void insert_left_endpoint(Trapezoid* trapezoid, const Line& l, Trapezoid*& Y, Trapezoid*& Z);
	void insert_no_segment_endpoint(Trapezoid* trapezoid, const Line& l, Trapezoid*& Y, Trapezoid*& Z);
	void insert_right_endpint(Trapezoid* trapezoid, const Line& l, Trapezoid*& Y, Trapezoid*& Z);
	LeafNode* queryNode(const Point& p);
	Trapezoid* nextTrapezoid(Trapezoid* trapezoid, const Line& l);
};


#endif