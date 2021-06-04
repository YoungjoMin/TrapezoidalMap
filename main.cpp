#include "trapezoidalMap.hpp"
#include "input.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>

//all leaf node is accessible ie) there exists point p s.t. search tree finds that leaf node by p
//leaf node's #parent <=4 // XNode by leftp, rightp, or YNode by top, bottom are the whole candidate

void getAnaylsis(const std::vector<Line>& lines, double bd) {
	auto start = std::chrono::high_resolution_clock::now();
	TrapezoidalMap tm(Point(-bd, -bd), Point(bd, bd));
	for (const Line& l : lines) {
		tm.insert(l);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> sec = end - start;
	int maxDepth = tm.maxDepth();
	std::cout << "time : " << sec.count()<< '\n';
	std::cout << "max depth : " << maxDepth << '\n';
}

void test(const std::vector<Line>& lines, const std::vector<Point>& pts, double bd) {
	TrapezoidalMap tm(Point(-bd, -bd),Point(bd,bd));
	for (const Line& l : lines) {
		tm.insert(l);
	}
	for (const Point& pt : pts) {
		auto trapezoid = tm.query(pt);
		std::cout << "query point : " << pt << '\n';
		std::cout << *trapezoid << "\n\n";
	}
	std::cout << "Max depth : " << tm.maxDepth() << "\n\n\n";
}

int testcase1() //pass
{
	std::vector<Line> lines;
	std::vector<Point> pts;

	for (int i = 9000; i > 0; i -= 10) {
		lines.push_back(Line(Point(-i, i), Point(i, i)));
	}

	pts.push_back(Point(0, 55));
	pts.push_back(Point(55, 55));
	pts.push_back(Point(0, -55));
	pts.push_back(Point(75, 20));
	
	test(lines, pts, 10000);
	return 0;
}

int testcase2() //pass
{
	std::vector<Line> lines;

	lines.push_back(Line(Point(-4, 2), Point(0, 4)));
	lines.push_back(Line(Point(-5, -2), Point(2, 0)));
	lines.push_back(Line(Point(-2, 1), Point(6, 2)));

	std::vector<Point> pts = { Point(-1,0), Point(4,0),Point(-2,6) };

	test(lines, pts, 100);
	return 0;
}

int testcase3() //pass
{
	std::vector<Line> lines;

	lines.push_back(Line(Point(-8, 10), Point(9, 10)));
	lines.push_back(Line(Point(-11, 3), Point(0, 3)));
	lines.push_back(Line(Point(4, 3), Point(13, 3)));
	lines.push_back(Line(Point(-4,7), Point(6, 7)));
	lines.push_back(Line(Point(-13, 5), Point(17, 5)));
	lines.push_back(Line(Point(15, 7), Point(18, 7)));

	std::vector<Point> pts;
	pts.push_back({ -6,1 });
	pts.push_back({ -1,6 });
	pts.push_back({ 5,8 });
	pts.push_back({ 16,6 });

	//test(lines, pts, 100);
	getAnaylsis(lines, 100);

	return 0;
}


int testcase4()
{
	std::vector<Line> lines;

	lines.push_back(Line({ {-8,8}, {8,8} }));
	lines.push_back(Line({ {-1,7}, {10,7} }));
	lines.push_back(Line({ {7,5}, {14,5} }));
	lines.push_back(Line({ {-4,4}, {1,4} }));
	lines.push_back(Line({ {-3,3}, {11,3} }));
	lines.push_back(Line({ {-11,5}, {12,-6} }));
	lines.push_back(Line({ {-12,-2}, {-2,-2} }));
	lines.push_back(Line({ {-4,-3}, {3,-3} }));
	lines.push_back(Line({ {-10,-4}, {4,-4} }));

	std::vector<Point> pts;
	pts.push_back({ -12.5,10.5 });
	pts.push_back({ 10.5,9.5 });
	pts.push_back({ 1.5,8.5 });
	pts.push_back({ 7.5,7.5 });
	pts.push_back({ -7.5,5.5 });
	pts.push_back({ 9.5,5.5 });
	pts.push_back({ -1.5,5});
	pts.push_back({ 0.5,3.5 });
	pts.push_back({ 6.5,3.5 });
	pts.push_back({ 9.5,2.5 });
	pts.push_back({ -0.5,1.5 });
	pts.push_back({ 11.5,1.5 });
	pts.push_back({ -5,1 });
	pts.push_back({ 0,-2 });
	pts.push_back({ -7.5,-3.5 });
	pts.push_back({ -0.5,-3.5 });
	pts.push_back({ 5.5,-5.5 });

	//test(lines, pts, 100);
	getAnaylsis(lines, 100);
	return 0;

}


int main_input()
{
	std::vector<Line> lines;
	genInput(15,lines);
	printInput(lines);
	return 0;
}

int main() {
	std::vector<Line> lines;
	scanInput(lines);
	//makeInputRandom(lines);
	makeInputAdversarial_sorting(lines);
	getAnaylsis(lines, 50000);
	return 0;
}
