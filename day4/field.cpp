#include "field.h"

Field::Field(const FieldType fieldType)  : mFieldType(fieldType) {
}

void Field::setValue(const std::string& value) {
	mIsSet = true;
	mValue = value;
}

bool Field::hasValidPresence() const {
	if (mFieldType == FieldType::MANDATORY) {
		return mIsSet;
	}
	return true;
}

bool Field::hasValidValue() const {
	if (hasValidPresence()) {
		return validateValue(mValue);
	}
	return false;
}
