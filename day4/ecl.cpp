#include <array>
#include "ecl.h"

bool Ecl::validateValue(const std::string& value) const {
	const std::array<std::string, 7> validColors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
	if (std::find(validColors.begin(), validColors.end(), value) == validColors.end()) {
		return false;
	}
	return true;
}
