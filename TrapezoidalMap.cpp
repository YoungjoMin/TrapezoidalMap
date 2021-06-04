#include "trapezoidalMap.hpp"


bool Point::isLeft(const Point& p) const {
	return x < p.x;
}
bool Point::isSame(const Point& p) const {
	return std::abs(x - p.x) < eps && std::abs(y - p.y) < eps;
}
Point operator-(const Point& p1, const Point& p2) {
	return Point(p1.x - p2.x, p1.y - p2.y);
}
Point operator+(const Point& p1, const Point& p2) {
	return Point(p1.x + p2.x, p1.y + p2.y);
}
Point Point::operator*(double s) {
	return Point(x * s, y * s);
}
Point Point::normalize() {
	double s = std::sqrt(x * x + y * y);
	return (*this) * s;
}
Point lefterPoint(const Point& p1, const Point& p2) {
	return p1.isLeft(p2) ? p1 : p2;
}
std::ostream& operator<<(std::ostream& o, const Point& t) {
	o << "( " << t.x << " , " << t.y << " )";
	return o;
}


Line::Line(const Point& pl_, const Point& pr_) {
	pl = pl_, pr = pr_;
	if (pr.isLeft(pl)) std::swap(pl, pr);
}
bool Line::isUpper(const Point& p) const {
	Point t1 = pr - pl, t2 = p - pl;
	return (t1.x * t2.y - t1.y * t2.x) < eps;
}
std::ostream& operator<<(std::ostream& o, const Line& l) {
	o <<"[ "<<l.pl << " -> " << l.pr << " ]";
	return o;

}

bool Line::IsPtEndpoint(const Point& p) const {
	return pl.isSame(p) || pr.isSame(p);
}

int TNode::maxDepth() {
	if (isLeaf()) return 0;
	return std::max(lc->maxDepth(), rc->maxDepth()) + 1;
}

bool XNode::isLeaf() const {
	return false;
}
TNode* XNode::query(const Point& pt) {
	return p.isLeft(pt) ? rc : lc;
}


bool YNode::isLeaf() const{
	return false;
}
TNode* YNode::query(const Point& pt) {
	return l.isUpper(pt) ? rc : lc;
}

LeafNode::LeafNode(Trapezoid* t_) {
	t = t_;
	t_->node = this;
	lc = rc = NULL;
}

bool LeafNode::isLeaf() const{
	return true;
}
TNode* LeafNode::query(const Point& pt) {
	return this;
}

Trapezoid::Trapezoid(Line top, Line bottom, Point leftp, Point rightp) : top(top), bottom(bottom), leftp(leftp), rightp(rightp) {
	lowerleft = upperleft = lowerright = upperright = NULL;
}

bool Trapezoid::isInside(const Point& pt) {
	return top.isUpper(pt)
		&& !bottom.isUpper(pt)
		&& leftp.isLeft(pt)
		&& !rightp.isLeft(pt);
}
void Trapezoid::updateLeftTrapezoid(Trapezoid* prv, Trapezoid* cur) {
	if (lowerleft == prv) lowerleft = cur;
	if (upperleft == prv) upperleft = cur;
}
void Trapezoid::updateRightTrapezoid(Trapezoid* prv, Trapezoid* cur) {
	if (lowerright == prv) lowerright = cur;
	if (upperright == prv) upperright = cur;
}
std::ostream& operator<<(std::ostream& o, const Trapezoid& t) {
	o << "top : " << t.top << '\n';
	o << "bottom : " << t.bottom << '\n';
	o << "leftp : " << t.leftp << '\n';
	o << "rightp : " << t.rightp<< '\n';
	return o;
}

int TrapezoidalMap::maxDepth() {
	return root->maxDepth();
}

TrapezoidalMap::TrapezoidalMap(const Point& bl, const Point& tr) {
	Point br(tr.x, bl.y), tl(bl.x, tr.y);

	Trapezoid* t = new Trapezoid(Line(tl,tr), Line(bl, br), bl,tr);

	LeafNode* leaf = new LeafNode(t);
	root = leaf;
	leaf->parents.push_back(&root);
}

TrapezoidalMap::~TrapezoidalMap() {
	
	std::set<TNode*> occur;
	std::queue<TNode*> q;
	q.push(root); occur.insert(root);
	while (!q.empty()) {
		TNode* cur = q.front();
		q.pop();
		if (cur->isLeaf()) continue;
		if (occur.find(cur->lc) != occur.end()) {
			q.push(cur->lc);
			occur.insert(cur->lc);
		}
		if (occur.find(cur->rc) != occur.end()) {
			q.push(cur->rc);
			occur.insert(cur->rc);
		}
	}
	for (TNode* tmp : occur) delete tmp; //only nodes are allocated in heap
}

LeafNode* TrapezoidalMap::queryNode(const Point& p) {
	TNode* cur = root;
	while (!cur->isLeaf()) {
		cur = cur->query(p);
	}

	return (LeafNode*)cur;
}


Trapezoid* TrapezoidalMap::query(const Point& p) {
	return queryNode(p)->t;
}

void TrapezoidalMap::insert(const Line& l) {
	//Point dl = ((l.pr - l.pl).normalize()) * eps;
	const Point& pl = l.pl;
	const Point& pr = l.pr;

	Trapezoid* tl = query(pl), * ntl;
	Trapezoid* Y = NULL, *Z = NULL;
	if (tl->isInside(pr)) {
		insert_two_segment_endpoint(tl, l);
		return;
	}

	ntl = nextTrapezoid(tl, l);
	insert_left_endpoint(tl, l, Y, Z);
	tl = ntl;

	while (!tl->isInside(pr)) {
		ntl = nextTrapezoid(tl, l);
		insert_no_segment_endpoint(tl, l, Y, Z);

		tl = ntl;
	}
	insert_right_endpint(tl, l, Y, Z);

	return;
}

void TrapezoidalMap::insert_two_segment_endpoint(Trapezoid* A, const Line& s) 
{
	const Point& p = s.pl, & q = s.pr;

	Trapezoid* U = new Trapezoid(A->top, A->bottom, A->leftp, p);
	Trapezoid* Y = new Trapezoid(A->top, s, p, q);
	Trapezoid* Z = new Trapezoid(s, A->bottom, p, q);
	Trapezoid* X = new Trapezoid(A->top, A->bottom, q, A->rightp);

	Y->lowerleft = Y->upperleft = Z->lowerleft = Z->upperleft = U;
	Y->lowerright = Y->upperright = Z->lowerright = Z->upperright = X;
	X->lowerleft = U->lowerright = Z;
	X->upperleft = U->upperright = Y;
	X->upperright = A->upperright;
	X->lowerright = A->lowerright;
	U->upperleft = A->upperleft;
	U->lowerleft = A->lowerleft;

	if (A->lowerright != NULL) A->lowerright->updateLeftTrapezoid(A, X);
	if (A->upperright != NULL) A->upperright->updateLeftTrapezoid(A, X);
	if (A->lowerleft != NULL) A->lowerleft->updateRightTrapezoid(A, U);
	if (A->upperleft != NULL) A->upperleft->updateRightTrapezoid(A, U);

	XNode* pnode = new XNode(p);
	XNode* qnode = new XNode(q);
	YNode* snode = new YNode(s);

	LeafNode* originalNode = (LeafNode *)A->node;
	LeafNode* Unode = new LeafNode(U);
	LeafNode* Xnode = new LeafNode(X);
	LeafNode* Ynode = new LeafNode(Y);
	LeafNode* Znode = new LeafNode(Z);

	pnode->lc = Unode;
	pnode->rc = qnode;
	qnode->lc = snode;
	qnode->rc = Xnode;
	snode->lc = Ynode;
	snode->rc = Znode;

	Unode->parents.push_back(&pnode->lc);
	Xnode->parents.push_back(&qnode->rc);
	Ynode->parents.push_back(&snode->lc);
	Znode->parents.push_back(&snode->rc);
	for (TNode** tmp: originalNode->parents) {
		*tmp = pnode;
	}
	
	delete originalNode;
	delete A;
}

void TrapezoidalMap::insert_left_endpoint(Trapezoid* A, const Line& s, Trapezoid*& pY, Trapezoid*& pZ) {
	const Point& p = s.pl, &q = s.pr;
	
	Trapezoid* X = new Trapezoid(A->top, A->bottom, A->leftp, p);
	Trapezoid* Y = new Trapezoid(A->top, s, p, Point()); //may not know rightp at this moment
	Trapezoid* Z = new Trapezoid(s, A->bottom, p, Point()); //may not know rightp at this moment
	

	X->upperright = Y;
	X->lowerright = Z;
	X->lowerleft = A->lowerleft;
	X->upperleft = A->upperleft;
	if (A->lowerleft != NULL) A->lowerleft->updateRightTrapezoid(A, X);
	if (A->upperleft != NULL) A->upperleft->updateRightTrapezoid(A, X);

	Y->upperleft = Y->lowerleft = X;
	Z->upperleft = Z->lowerleft = X;


	if (s.isUpper(A->rightp)) { //Z is the next A
		Z->rightp = A->rightp;
		//Z->upperright = NULL;
		//Z->lowerright = NULL;
		if (A->rightp.isSame(A->bottom.pr) == false) {
			Z->lowerright = A->lowerright;
			if (A->lowerright != NULL) A->lowerright->updateLeftTrapezoid(A, Z);
		}

		//Y-> updated next time both right is NULL
		//Z-> both right is NULL or only upperright is NULL
	}
	else { //Y is the next A
		Y->rightp = A->rightp;
		//Y->upperright = NULL;
		//Y->lowerright = NULL;
		if (A->rightp.isSame(A->top.pr) == false) {
			Y->upperright = A->upperright;
			if (A->upperright != NULL)A->upperright->updateLeftTrapezoid(A, Y);
		}

		//Y-> both right is NULL or only upperright is NULL
		//Z-> updated next time both right is NULL
	}
	


	XNode* pnode = new XNode(p);
	YNode* snode = new YNode(s);

	LeafNode* originalNode = (LeafNode*)A->node;
	LeafNode* Xnode = new LeafNode(X);
	LeafNode* Ynode = new LeafNode(Y);
	LeafNode* Znode = new LeafNode(Z);

	pnode->lc = Xnode;
	pnode->rc = snode;
	snode->lc = Ynode;
	snode->rc = Znode;

	Xnode->parents.push_back(&pnode->lc);
	Ynode->parents.push_back(&snode->lc);
	Znode->parents.push_back(&snode->rc);


	for (TNode** tmp : originalNode->parents) {
		*tmp = pnode;
	}

	pY = Y;
	pZ = Z;
	delete originalNode;
	delete A;
}


void TrapezoidalMap::insert_no_segment_endpoint(Trapezoid* A, const Line& s, Trapezoid*& pY, Trapezoid*& pZ) {
	const Point& p = s.pl, & q = s.pr;
	Trapezoid* Y = s.isUpper(A->leftp) ? pY : new Trapezoid(A->top, s, A->leftp, Point()); //for some case may not know rightp of Y
	Trapezoid* Z = s.isUpper(A->leftp) ? new Trapezoid(s, A->bottom, A->leftp, Point()) : pZ; //for some case may not know rightp of Z


	if (s.isUpper(A->leftp)) {
		Z->upperleft = Z->lowerleft = pZ;
		
		if (pZ->lowerright == NULL) {
			Z->lowerleft = A->lowerleft;
			if (A->lowerleft != NULL) A->lowerleft->updateRightTrapezoid(A, Z);
		}
		pZ->updateRightTrapezoid(NULL, Z);
	}
	else {
		Y->upperleft = Y->lowerleft = pY;

		if (pY->upperright == NULL) {
			Y->upperleft = A->upperleft;
			if (A->upperleft != NULL) A->upperleft->updateRightTrapezoid(A, Y);
		}
		pY->updateRightTrapezoid(NULL, Y);
	}


	if (s.isUpper(A->rightp)) { //Z is the next A
		Z->rightp = A->rightp;
		//Z->upperright = NULL;
		//Z->lowerright = NULL;
		if (A->rightp.isSame(A->bottom.pr) == false) {
			Z->lowerright = A->lowerright;
			if (A->lowerright != NULL) A->lowerright->updateLeftTrapezoid(A, Z);
		}

		//Y-> updated next time both right is NULL
		//Z-> both right is NULL or only upperright is NULL
	}
	else { //Y is the next A
		Y->rightp = A->rightp;
		//Y->upperright = NULL;
		//Y->lowerright = NULL;
		if (A->rightp.isSame(A->top.pr) == false) {
			Y->upperright = A->upperright;
			if (A->upperright != NULL)A->upperright->updateLeftTrapezoid(A, Y);
		}

		//Y-> both right is NULL or only upperright is NULL
		//Z-> updated next time both right is NULL
	}

	YNode* snode = new YNode(s);
	LeafNode* originalNode = (LeafNode*)A->node;
	LeafNode* Ynode = s.isUpper(A->leftp) ? (LeafNode*)Y->node : new LeafNode(Y);
	LeafNode* Znode = s.isUpper(A->leftp) ? new LeafNode(Z) : (LeafNode*)Z->node;

	snode->lc = Ynode;
	snode->rc = Znode;
	Ynode->parents.push_back(&snode->lc);
	Znode->parents.push_back(&snode->rc);

	for (TNode** tmp : originalNode->parents) {
		*tmp = snode;
	}
	pY = Y;
	pZ = Z;
	delete originalNode;
	delete A;
}

void TrapezoidalMap::insert_right_endpint(Trapezoid* A, const Line& s, Trapezoid*& pY, Trapezoid*& pZ) {
	const Point& p = s.pl, & q = s.pr;

	Trapezoid* Y = s.isUpper(A->leftp) ? pY : new Trapezoid(A->top, s, A->leftp, q);
	Trapezoid* Z = s.isUpper(A->leftp) ? new Trapezoid(s, A->bottom, A->leftp, q) : pZ;
	Trapezoid* X = new Trapezoid(A->top, A->bottom, q, A->rightp);


	X->upperleft = Y;
	X->lowerleft = Z;
	X->lowerright = A->lowerright;
	X->upperright = A->upperright;

	Y->upperright = Y->lowerright = X;
	Z->upperright = Z->lowerright = X;
	Y->rightp = Z->rightp = q;

	if (s.isUpper(A->leftp)) {
		Z->upperleft = Z->lowerleft = pZ;

		if (pZ->lowerright == NULL) {
			Z->lowerleft = A->lowerleft;
			if (A->lowerleft != NULL) A->lowerleft->updateRightTrapezoid(A, Z);
		}
		pZ->updateRightTrapezoid(NULL, Z);
	}
	else {
		Y->upperleft = Y->lowerleft = pY;

		if (pY->upperright == NULL) {
			Y->upperleft = A->upperleft;
			if (A->upperleft != NULL) A->upperleft->updateRightTrapezoid(A, Y);
		}
		pY->updateRightTrapezoid(NULL, Y);
	}

	if (A->lowerright != NULL)A->lowerright->updateLeftTrapezoid(A, X);
	if (A->upperright != NULL)A->upperright->updateLeftTrapezoid(A, X);

	XNode* qnode = new XNode(q);
	YNode* snode = new YNode(s);

	LeafNode* originalNode = (LeafNode*)A->node;
	LeafNode* Xnode = new LeafNode(X);
	LeafNode* Ynode = s.isUpper(A->leftp) ? (LeafNode*)Y->node : new LeafNode(Y);
	LeafNode* Znode = s.isUpper(A->leftp) ? new LeafNode(Z) : (LeafNode*)Z->node;

	qnode->lc = snode;
	qnode->rc = Xnode;
	snode->lc = Ynode;
	snode->rc = Znode;

	Xnode->parents.push_back(&qnode->rc);
	Ynode->parents.push_back(&snode->lc);
	Znode->parents.push_back(&snode->rc);


	for (TNode** tmp : originalNode->parents) {
		*tmp = qnode;
	}

	pY = Y;
	pZ = Z;
	delete originalNode;
	delete A;
}

Trapezoid* TrapezoidalMap::nextTrapezoid(Trapezoid* node, const Line& l) {
	return l.isUpper(node->rightp) ? node->upperright : node->lowerright;
}