#include "byr.h"

bool Byr::validateValue(const std::string& value) const {
	const int intValue = std::stoi(value);
	return intValue >= 1920 && intValue <= 2002;
}
