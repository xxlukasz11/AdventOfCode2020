#pragma once

#include "field.h"

class Iyr : public Field {
private:
	bool validateValue(const std::string& value) const override;
};

