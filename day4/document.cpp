#include <sstream>
#include "document.h"

Document::Document() {
	mFieldMap["byr"] = std::make_unique<Byr>();
	mFieldMap["iyr"] = std::make_unique<Iyr>();
	mFieldMap["eyr"] = std::make_unique<Eyr>();
	mFieldMap["hgt"] = std::make_unique<Hgt>();
	mFieldMap["hcl"] = std::make_unique<Hcl>();
	mFieldMap["ecl"] = std::make_unique<Ecl>();
	mFieldMap["pid"] = std::make_unique<Pid>();
	mFieldMap["cid"] = std::make_unique<Cid>();
}

void Document::setField(const std::string& fieldName, const std::string& value) {
	mFieldMap[fieldName]->setValue(value);
}

bool Document::hasAllRequiredFields() const {
	for (const auto& entry : mFieldMap) {
		if (!entry.second->hasValidPresence()) {
			return false;
		}
	}
	return true;
}

bool Document::hasValidFields() const {
	for (const auto& entry : mFieldMap) {
		if (!entry.second->hasValidValue()) {
			return false;
		}
	}
	return true;
}

Document Document::parse(const std::string& str) {
	std::istringstream stream(str);
	Document document;
	for (std::string word; stream >> word;) {
		auto separator = word.find(":");
		auto key = word.substr(0, separator);
		auto value = word.substr(separator + 1);
		document.setField(key, value);
	}
	return document;
}
