#include "iyr.h"

bool Iyr::validateValue(const std::string& value) const {
	const int intValue = std::stoi(value);
	return intValue >= 2010 && intValue <= 2020;
}
