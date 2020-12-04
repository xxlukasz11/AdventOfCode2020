#pragma once

#include "field.h"

class Ecl : public Field {
private:
	bool validateValue(const std::string& value) const override;
};

