#pragma once

#include "field.h"

class Byr : public Field {
private:
	bool validateValue(const std::string& value) const override;
};

