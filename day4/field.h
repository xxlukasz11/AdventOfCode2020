#pragma once

#include <string>

enum class FieldType {
	MANDATORY,
	OPTIONAL
};

class Field {
public:
	Field(const FieldType fieldType = FieldType::MANDATORY);
	void setValue(const std::string& value);
	bool hasValidPresence() const;
	bool hasValidValue() const;

protected:
	virtual bool validateValue(const std::string& value) const = 0;

private:
	FieldType mFieldType;
	bool mIsSet{ false };
	std::string mValue;
};
