#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../common/fileReader.h"

using DataType = std::vector<int>;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextWord(line);) {
		const auto value = std::stoi(line);
		data.push_back(value);
	}
	return data;
}

void partOne(const DataType& data) {
	int currentJoltage = 0;
	int singleJolt = 0;
	int threeJolts = 0;
	for (const auto adapter : data) {
		if (adapter - currentJoltage == 1) {
			++singleJolt;
		} else if (adapter - currentJoltage == 3) {
			++threeJolts;
		}
		currentJoltage = adapter;
	}
	std::cout << "Part one: " << singleJolt * threeJolts << std::endl;
}

std::vector<int> getNumberOfPossibleTransitions(const DataType& data) {
	std::vector<int> possibilities;
	for (int i = 0; i < data.size()-1; ++i) {
		int numberOfPossibilities = 0;
		for (int j = i + 1; j < data.size() && data[j] - data[i] <= 3; ++j) {
			++numberOfPossibilities;
		}
		possibilities.push_back(numberOfPossibilities);
	}
	return possibilities;
}

void partTwo(DataType data) {
	data.insert(data.begin(), 0);
	const auto transitions = getNumberOfPossibleTransitions(data);
	std::vector<long long> noOfNodes(transitions.size(), 0);
	noOfNodes[0] = 1;
	for (int i = 0; i < transitions.size() - 1; ++i) {
		const auto val = transitions[i];
		if (val >= 1) {
			noOfNodes[i + 1] += noOfNodes[i];
		}
		if (val >= 2) {
			noOfNodes[i + 2] += noOfNodes[i];
		}
		if (val >= 3) {
			noOfNodes[i + 3] += noOfNodes[i];
		}
	}
	std::cout << "Part two: " << noOfNodes.back() << std::endl;
}

int main() {
	DataType data = read();
	std::sort(data.begin(), data.end());
	data.push_back(data.back() + 3);
	partOne(data);
	partTwo(data);
	return 0;
}