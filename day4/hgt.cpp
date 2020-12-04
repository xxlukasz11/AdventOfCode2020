#include <sstream>
#include "hgt.h"

bool Hgt::validateValue(const std::string& value) const {
	std::istringstream stream(value);
	int height;
	stream >> height;
	const int heightLength = std::to_string(height).size();
	const std::string unit = value.substr(heightLength);
	if (unit == "cm") {
		return height >= 150 && height <= 193;
	}
	else if (unit == "in") {
		return height >= 59 && height <= 76;
	}
	else {
		return false;
	}
}
