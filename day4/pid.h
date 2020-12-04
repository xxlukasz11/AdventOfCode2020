#pragma once

#include "field.h"

class Pid : public Field {
private:
	bool validateValue(const std::string& value) const override;
};

