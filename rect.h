#ifndef RECT_H
#define RECT_H

#include "point2D.h"

class Rect {
public:
	Rect() {}

	Rect(int x1, int y1, int x2, int y2) : _p1(Point2D(x1,y1)),
		_p2(Point2D(x2,y2)){}
	Rect(const Point2D p1,const Point2D p2) {	setP1(p1)->setP2(p2); }

	~Rect() {}

	Point2D p1() const { return _p1; }
	Point2D p2() const { return _p2; }

	Rect* setP1(const Point2D p1) { _p1.setX(p1.x())->setY(p1.y()); return this; }
	Rect* setP2(const Point2D p2) { _p2.setX(p2.x())->setY(p2.y()); return this; }

private:
	Point2D _p1;
	Point2D _p2;
};

#endif // RECT_H
