#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <regex>

struct Rule {
	int index;
	std::string str;
};

struct Data {
	std::vector<Rule> rules;
	std::vector<std::string> messages;
};

using DataType = Data;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextLine(line) && !line.empty();) {
		Rule rule;
		auto firstSpace = line.find(' ');
		
		rule.index = std::stoi(line.substr(0, firstSpace - 1));
		auto secondPart = line.substr(firstSpace + 1);
		if (secondPart.size() > 1 && (secondPart[1] == 'a' || secondPart[1] == 'b')) {
			rule.str = secondPart[1];
		}
		else {
			rule.str = secondPart;
		}
		data.rules.push_back(rule);
	}

	for (std::string line; reader.nextLine(line);) {
		data.messages.push_back(line);
	}
	return data;
}

std::regex createRegexForRuleZero(const std::vector<Rule>& rules) {
	bool changed = true;
	auto ruleZero = rules.front().str;
	int loops = 0;
	while (changed) {
		changed = false;
		std::istringstream stream(ruleZero);
		std::ostringstream out;
		for (std::string val; stream >> val;) {
			if (val[0] >= '0' && val[0] <= '9') {
				changed = true;
				int index = std::stoi(val);
				if (index == 8 && loops > 0) {
					out << " " << 42 << " ";
				}
				else if (index == 11 && loops > 1) {
					out << " " << 42 << " " << 31 << " ";
				}
				else {
					auto str = rules[index].str;
					if (str == "a" || str == "b") {
						out << " " << rules[index].str << " ";
					}
					else {
						out << " ( " << rules[index].str << " ) ";
					}
				}
			}
			else {
				out << " " << val << " ";
			}
		}
		ruleZero = out.str();
		if (ruleZero.size() > 10000) {
			++loops;
		}
	}

	// remove spaces
	std::istringstream stream(ruleZero);
	ruleZero = "";
	for (std::string word; stream >> word;) {
		ruleZero += word;
	}

	return std::regex("^" + ruleZero + "$", std::regex_constants::ECMAScript);
}

int countValidMessages(const std::vector<std::string>& messages, const std::regex& reg) {
	int count = 0;
	for (const std::string msg : messages) {
		if (std::regex_search(msg, reg)) {
			++count;
		}
	}
	return count;
}

void partOne(const DataType& data) {
	std::regex reg = createRegexForRuleZero(data.rules);
	int count = countValidMessages(data.messages, reg);

	std::cout << "Part one: " << count << std::endl;
}

void partTwo(const DataType& data) {
	auto rules = data.rules;
	rules[8].str = "42 | 42 8";
	rules[11].str = "42 31 | 42 11 31";
	
	std::regex reg = createRegexForRuleZero(rules);
	int count = countValidMessages(data.messages, reg);
	std::cout << "Part two: " << count << std::endl;
}

int main() {
	auto data = read();
	std::sort(data.rules.begin(), data.rules.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.index < rhs.index;
	});
	partOne(data);
	partTwo(data);
	return 0;
}