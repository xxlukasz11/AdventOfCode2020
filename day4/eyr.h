#pragma once

#include "field.h"

class Eyr : public Field {
private:
	bool validateValue(const std::string& value) const override;
};

