#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <unordered_map>

using DataType = std::vector<int>;

int lastNumberAtStep(const DataType& data, const int targetStep) {
	std::unordered_map<int, int> registry;
	for (int i = 0; i < data.size() - 1; ++i) {
		registry[data[i]] = i + 1;
	}

	int lastNumber = data.back();
	for (int currentIndex = data.size(); currentIndex < targetStep; ++currentIndex) {
		const int lastOccurence = registry[lastNumber];
		registry[lastNumber] = currentIndex;
		if (lastOccurence == 0) {
			lastNumber = 0;
		}
		else {
			lastNumber = currentIndex - lastOccurence;
		}
	}
	return lastNumber;
}

void partOne(const DataType& data) {
	const int lastNumber = lastNumberAtStep(data, 2020);
	std::cout << "Part one: " << lastNumber << std::endl;
}

void partTwo(const DataType& data) {
	const int lastNumber = lastNumberAtStep(data, 30000000);
	std::cout << "Part two: " << lastNumber << std::endl;
}

int main() {
	DataType data{ 13,0,10,12,1,5,8 };
	partOne(data);
	partTwo(data);
	return 0;
}