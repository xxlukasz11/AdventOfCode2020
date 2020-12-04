#pragma once

#include "field.h"

class Cid : public Field {
public:
	Cid();

private:
	bool validateValue(const std::string& value) const override;
};

