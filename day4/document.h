#pragma once

#include <unordered_map>
#include <memory>
#include "byr.h"
#include "eyr.h"
#include "ecl.h"
#include "cid.h"
#include "hgt.h"
#include "hcl.h"
#include "pid.h"
#include "iyr.h"

class Document {
public:
	Document();
	void setField(const std::string& fieldName, const std::string& value);
	bool hasAllRequiredFields() const;
	bool hasValidFields() const;

	static Document parse(const std::string& str);

private:
	std::unordered_map<std::string, std::shared_ptr<Field>> mFieldMap;
};

