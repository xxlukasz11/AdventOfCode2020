#pragma once

#include "field.h"

class Hcl : public Field {
private:
	bool validateValue(const std::string& value) const override;
};

