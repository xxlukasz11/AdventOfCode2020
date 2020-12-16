#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

struct Range {
	int min;
	int max;
};

struct Rule {
	std::string name;
	Range range1;
	Range Range2;
};

using TicketType = std::vector<int>;

struct File {
	std::vector<Rule> rules;
	TicketType myTicket;
	std::vector<TicketType> tickets;
};

Range parseRange(const std::string& str) {
	auto separator = str.find("-");
	const auto min = std::stoi(str.substr(0, separator));
	const auto max = std::stoi(str.substr(separator+1));
	return { min, max };
}

TicketType parseTicket(std::string str) {
	std::istringstream stream(str);
	TicketType ticket;
	for (int value; stream >> value;) {
		ticket.push_back(value);
		if (stream.peek() == ',') {
			stream.ignore();
		}
	}
	return ticket;
}

File read() {
	common::FileReader reader("input.txt");
	File file;
	for (std::string line; reader.nextLine(line) && line != "";) {
		const auto colon = line.find(":");
		std::string name = line.substr(0, colon);
		std::istringstream str(line.substr(colon + 1));
		std::string word;
		str >> word;
		Range range1 = parseRange(word);
		str >> word;
		str >> word;
		Range range2 = parseRange(word);
		file.rules.push_back(Rule{ name, range1, range2 });
	}
	
	reader.nextLine();
	std::string line = reader.nextLine();
	file.myTicket = parseTicket(line);
	reader.nextLine();
	reader.nextLine();

	for (std::string line; reader.nextLine(line);) {
		const auto ticket = parseTicket(line);
		file.tickets.push_back(ticket);
	}

	return file;
}

bool validateFieldValue(const Rule& rule, int value) {
	const auto& [_, range1, range2] = rule;
	if ((value >= range1.min && value <= range1.max) ||
		(value >= range2.min && value <= range2.max)) {
		return true;
	}
	return false;
}

std::pair<int, bool> calculateErrorRate(const std::vector<Rule>& rules, TicketType ticket) {
	bool invalidWithZero = false;
	int errorSum = 0;
	for (const auto value : ticket) {
		bool valid = false;
		for (const auto& rule : rules) {
			if (validateFieldValue(rule, value)) {
				valid = true;
				break;
			}
		}
		if (!valid) {
			errorSum += value;
			if (value == 0) {
				invalidWithZero = true;
			}
		}
	}
	return { errorSum, invalidWithZero };
}

bool validateRuleOrder(const std::vector<Rule>& rules, const std::vector<TicketType>& tickets) {
	const auto numberOfFields = tickets.front().size();
	for (const auto& ticket : tickets) {
		for (int i = 0; i < numberOfFields; ++i) {
			if (!validateFieldValue(rules[i], ticket[i])) {
				return false;
			}
		}
	}
	return true;
}

std::vector<std::vector<int>> getIndexesOfMatchingRules(const std::vector<Rule>& rules, const std::vector<TicketType> fields) {
	std::vector<std::vector<int>> matchingRules(fields.size());
	for (int i = 0; i < fields.size(); ++i) {
		const auto& fieldValues = fields[i];
		for (int ruleIndex = 0; ruleIndex < rules.size(); ++ruleIndex) {
			bool valid = true;
			for (auto value : fieldValues) {
				if (!validateFieldValue(rules[ruleIndex], value)) {
					valid = false;
					break;
				}
			}
			if (valid) {
				matchingRules[i].push_back(ruleIndex);
			}
		}
	}
	return matchingRules;
}

std::vector<TicketType> transposeTicketsToFields(const std::vector<TicketType>& tickets) {
	std::vector<TicketType> fields(tickets.front().size());
	for (const auto& ticket : tickets) {
		for (int i = 0; i < ticket.size(); ++i) {
			fields[i].push_back(ticket[i]);
		}
	}
	return fields;
}

std::vector<Rule> mapRuleToIndex(std::vector<Rule> rules, const std::vector<TicketType>& tickets) {
	std::vector<TicketType> fields = transposeTicketsToFields(tickets);
	std::vector<std::vector<int>> matchingRules = getIndexesOfMatchingRules(rules, fields);

	std::vector<Rule> rulesInOrder(rules.size());
	bool found = true;
	while (found) {
		found = false;
		for (int i = 0; i < matchingRules.size(); ++i) {
			if (matchingRules[i].size() == 1) {
				found = true;
				const auto ruleIndex = matchingRules[i].front();
				rulesInOrder[i] = rules[ruleIndex];
				for (auto& rr : matchingRules) {
					rr.erase(std::remove(rr.begin(), rr.end(), ruleIndex), rr.end());
				}
				break;
			}
		}
	}
	return rulesInOrder;
}

void partOne(const File& data) {
	const auto& [rules, _, tickets] = data;
	int sum = 0;
	for (const auto& ticket : tickets) {
		sum += calculateErrorRate(rules, ticket).first;
	}

	std::cout << "Part one: " << sum << std::endl;
}

void partTwo(const File& data) {
	const auto& [rules, myTicket, tickets] = data;
	std::vector<TicketType> validTickets;
	for (const auto& ticket : tickets) {
		if (auto [rate, invalidZero] = calculateErrorRate(rules, ticket); rate == 0 && !invalidZero) {
			validTickets.push_back(ticket);
		}
	}

	const auto validRules = mapRuleToIndex(rules, validTickets);
	int64_t mul = 1;
	for (int i = 0; i < validRules.size(); ++i) {
		if (validRules[i].name.find("departure") != std::string::npos) {
			mul *= myTicket[i];
		}
	}
	
	std::cout << "Part two: " << mul << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}