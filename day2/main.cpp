#include <iostream>
#include <vector>
#include "../common/fileReader.h"

struct PasswordEntry {
	int minCount;
	int maxCount;
	char reqLetter;
	std::string password;
};

std::pair<int, int> parseRange(const std::string range) {
	auto separator = range.find('-');
	const auto min = range.substr(0, separator);
	auto max = range.substr(separator + 1);
	return { std::stoi(min), std::stoi(max) };
}

std::vector<PasswordEntry> prepareEntries() {
	std::vector<PasswordEntry> entries;
	common::FileReader reader("input.txt");
	for (std::string range; reader.nextWord(range);) {
		PasswordEntry entry;
		const auto [min, max] = parseRange(range);
		entry.minCount = min;
		entry.maxCount = max;
		entry.reqLetter = reader.nextWord().at(0);
		entry.password = reader.nextWord();
		entries.push_back(entry);
	}
	return entries;
}

void partOne(const std::vector<PasswordEntry>& entries) {
	int validCount = 0;
	for (const auto [min, max, letter, password] : entries) {
		const auto letterCount = std::count(password.begin(), password.end(), letter);
		if (letterCount >= min && letterCount <= max) {
			++validCount;
		}
	}
	std::cout << "Part 1 - Valid passwords: " << validCount << std::endl;
}

void partTwo(const std::vector<PasswordEntry>& entries) {
	int validCount = 0;
	for (const auto [min, max, letter, password] : entries) {
		if (max > password.size() || min < 1) {
			continue;
		}
		bool first = password[min - 1] == letter;
		bool second = password[max - 1] == letter;
		if (first ^ second) {
			++validCount;
		}
	}
	std::cout << "Part2 - Valid passwords: " << validCount << std::endl;
}

int main() {
	std::vector<PasswordEntry> entries = prepareEntries();
	partOne(entries);
	partTwo(entries);
	return 0;
}