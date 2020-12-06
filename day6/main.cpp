#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "../common/fileReader.h"

struct Group {
	std::string answers;
	int noOfPeople;
};

std::vector<Group> read() {
	common::FileReader reader("input.txt");
	std::vector<Group> groups;
	for (std::string answers; reader.nextLine(answers);) {
		std::string nextLine;
		int people = 1;
		while (reader.nextLine(nextLine) && nextLine != "") {
			answers += nextLine;
			++people;
		}
		groups.push_back(Group{ answers, people });
	}
	return groups;
}

void partOne(const std::vector<Group>& groups) {
	int count = 0;
	for (const auto& group : groups) {
		std::string answers = group.answers;
		std::sort(answers.begin(), answers.end());
		auto last = std::unique(answers.begin(), answers.end());
		answers.erase(last, answers.end());
		count += answers.size();
	}
	std::cout << "Part one - count: " << count << std::endl;
}

void partTwo(const std::vector<Group>& groups) {
	int count = 0;
	for (const auto& [answers, people] : groups) {
		std::unordered_map<char, int> countMap;
		for (auto c : answers) {
			++countMap[c];
		}
		for (const auto& [key, value] : countMap) {
			if (value == people) {
				++count;
			}
		}
	}
	std::cout << "Part two - count: " << count << std::endl;
}

int main() {
	std::vector<Group> groups = read();
	partOne(groups);
	partTwo(groups);
	return 0;
}