#include "cid.h"

Cid::Cid() : Field(FieldType::OPTIONAL) {
}

bool Cid::validateValue(const std::string& value) const {
	return true;
}
