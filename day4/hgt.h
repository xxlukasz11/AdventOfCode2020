#pragma once

#include "field.h"

class Hgt : public Field {
private:
	bool validateValue(const std::string& value) const override;
};

