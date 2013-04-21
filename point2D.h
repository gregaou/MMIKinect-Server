#ifndef POINT2D_H
#define POINT2D_H

class Point2D {

public:

	Point2D(){}
	Point2D(int x, int y) : _x(x), _y(y) {}
	~Point2D(){}

	int x() const { return _x; }
	int y() const { return _y; }

	Point2D* setX(const int x) { _x = x; return this; }
	Point2D* setY(const int y) { _y = y; return this; }

private:

	int _x;
	int _y;

};

#endif // POINT2D_H
