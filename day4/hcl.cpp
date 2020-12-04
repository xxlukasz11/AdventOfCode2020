#include "hcl.h"

bool Hcl::validateValue(const std::string& value) const {
	if (value.at(0) != '#' || value.size() != 7) {
		return false;
	}
	std::string hex = value.substr(1);
	for (auto letter : hex) {
		if ((letter - '0' > 9 || letter - '0' < 0) &&
			(letter - 'a' > 6 || letter - 'a' < 0)) {
			return false;
		}
	}
	return true;
}
