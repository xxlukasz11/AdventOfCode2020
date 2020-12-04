#include "eyr.h"

bool Eyr::validateValue(const std::string& value) const {
	const int intValue = std::stoi(value);
	return intValue >= 2020 && intValue <= 2030;
}
