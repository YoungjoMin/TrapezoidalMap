#include "trapezoidalMap.hpp"
#include "input.hpp"

static std::random_device rd;
static std::mt19937 mt(rd());

void makeInputRandom(std::vector<Line>& l) {
	std::shuffle(l.begin(), l.end(), mt);
}

void makeInputAdversarial_sorting(std::vector<Line>& l) {
	std::sort(l.begin(), l.end(), [](const Line& l1, const Line& l2) {
		return l1.pl.x > l2.pl.x;
	});
}

void genInput(int size, std::vector<Line>& l) {
	std::vector<double> xlist;
	std::vector<double> ylist;
	for (int i = 1; i <= size; i++) {
		xlist.push_back(i);
		xlist.push_back(-i);
		ylist.push_back(i);
	}
	std::shuffle(xlist.begin(), xlist.end(), mt);
	std::shuffle(ylist.begin(), ylist.end(), mt);
	for (int i = 0; i < size; i++) {
		double x1 = xlist[2 * i];
		double x2 = xlist[2 * i + 1];
		double y = ylist[i];
		if (x1 > x2) std::swap(x1, x2);

		l.push_back(Line(Point(x1, y), Point(x2, y)));
	}
}


void scanInput(std::vector<Line>& l) {
	int n;
	std::cin >> n;
	double x1, y1, x2, y2;
	for (int i = 0; i < n; i++) {
		std::cin >> x1 >> y1 >> x2 >> y2;
		l.push_back(Line(Point(x1, y1), Point(x2, y2)));
	}
}

void printInput(std::vector<Line>& lines) {
	std::cout << lines.size() << '\n';
	for (Line& l : lines) {
		std::cout << l.pl.x << ' ';
		std::cout << l.pl.y << ' ';
		std::cout << l.pr.x << ' ';
		std::cout << l.pr.y << '\n';
	}
}