#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <array>
#include "../common/fileReader.h"

class Document {
public:
	Document() {
		fieldPresence.emplace("byr", false);
		fieldPresence.emplace("iyr", false);
		fieldPresence.emplace("eyr", false);
		fieldPresence.emplace("hgt", false);
		fieldPresence.emplace("hcl", false);
		fieldPresence.emplace("ecl", false);
		fieldPresence.emplace("pid", false);
		fieldPresence.emplace("cid", false);
	}

	void setField(const std::string& fieldName, const std::string& value) {
		fieldPresence[fieldName] = true;
		fieldValues[fieldName] = value;
	}

	bool hasAllRequiredFields() const {
		for (const auto& entry : fieldPresence) {
			if (!entry.second && entry.first != "cid") {
				return false;
			}
		}
		return true;
	}

	bool hasValidFields() const {
		if (!hasAllRequiredFields()) {
			return false;
		}

		for (const auto& entry : fieldValues) {
			if (!validateField(entry.first, entry.second)) {
				return false;
			}
		}
		return true;
	}

private:
	bool validateField(const std::string& fieldName, const std::string& value) const {
		if (fieldName == "byr") {
			const int intValue = std::stoi(value);
			return intValue >= 1920 && intValue <= 2002;
		}
		if (fieldName == "iyr") {
			const int intValue = std::stoi(value);
			return intValue >= 2010 && intValue <= 2020;
		}
		if (fieldName == "eyr") {
			const int intValue = std::stoi(value);
			return intValue >= 2020 && intValue <= 2030;
		}
		if (fieldName == "hgt") {
			std::istringstream stream(value);
			int height;
			stream >> height;
			const int heightLength = std::to_string(height).size();
			const std::string unit = value.substr(heightLength);
			if (unit == "cm") {
				return height >= 150 && height <= 193;
			}
			else if (unit == "in") {
				return height >= 59 && height <= 76;
			}
			else {
				return false;
			}
		}
		if (fieldName == "hcl") {
			if (value.at(0) != '#' || value.size() != 7) {
				return false;
			}
			std::string hex = value.substr(1);
			for (auto letter : hex) {
				if ((letter - '0' > 9 || letter - '0' < 0) &&
					(letter - 'a' > 6 || letter - 'a' < 0)) {
					return false;
				}
			}
			return true;
		}
		if (fieldName == "ecl") {
			const std::array<std::string, 7> validColors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
			if (std::find(validColors.begin(), validColors.end(), value) == validColors.end()) {
				return false;
			}
			return true;
		}
		if (fieldName == "pid") {
			if (value.size() != 9) {
				return false;
			}
			for (auto letter : value) {
				if (letter - '0' > 9 || letter - '0' < 0) {
					return false;
				}
			}
			return true;
		}
		if (fieldName == "cid") {
			return true;
		}
		return false;
	}

	std::map<std::string, bool> fieldPresence;
	std::map<std::string, std::string> fieldValues;
};

Document parseDocument(const std::string& str) {
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

std::vector<Document> readDocuments() {
	common::FileReader reader("input.txt");
	std::vector<Document> documents;
	for (std::string line; reader.nextLine(line);) {
		std::string nextLine;
		while (reader.nextLine(nextLine) && nextLine != "") {
			line += " " + nextLine;
		}
		documents.push_back(parseDocument(line));
	}
	return documents;
}

void partOne(const std::vector<Document>& documents) {
	int validDocuments = 0;
	for (const auto& doc : documents) {
		if (doc.hasAllRequiredFields()) {
			++validDocuments;
		}
	}
	std::cout << "Part one - Valid documents: " << validDocuments << std::endl;
}

void partTwo(const std::vector<Document>& documents) {
	int validDocuments = 0;
	for (const auto& doc : documents) {
		if (doc.hasValidFields()) {
			++validDocuments;
		}
	}
	std::cout << "Part two - Valid documents: " << validDocuments << std::endl;
}

int main() {
	std::vector<Document> documents = readDocuments();
	partOne(documents);
	partTwo(documents);
	return 0;
}