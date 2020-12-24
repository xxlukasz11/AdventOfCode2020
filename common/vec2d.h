#pragma once

namespace common {

class Vec2d {
public:
	Vec2d(const int xx, const int yy);
	bool operator==(const Vec2d& vec) const;
	bool operator<(const Vec2d& vec) const;
	int getX() const;
	int getY() const;

private:
	int mX;
	int mY;
};

}