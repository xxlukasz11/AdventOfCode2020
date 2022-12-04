#include "pch.h"
#include "vec2d.h"

namespace common {

Vec2d::Vec2d(const int x, const int y) : mX(x), mY(y) {}

bool Vec2d::operator==(const Vec2d& vec) const {
	return vec.mX == mX && vec.mY == mY;
}

bool Vec2d::operator<(const Vec2d& vec) const {
	if (mX == vec.mX) {
		return mY < vec.mY;
	}
	return mX < vec.mX;
}

int Vec2d::getX() const {
	return mX;
}

int Vec2d::getY() const {
	return mY;
}

}