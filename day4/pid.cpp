#include "pid.h"

bool Pid::validateValue(const std::string& value) const {
	if (value.size() != 9) {
		return false;
	}
	for (auto letter : value) {
		if (letter - '0' > 9 || letter - '0' < 0) {
			return false;
		}
	}
	return true;
}
