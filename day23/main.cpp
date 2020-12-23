#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <sstream>

using DataType = std::list<int>;

DataType::iterator getNextCup(DataType& data, DataType::iterator currentCup) {
	std::advance(currentCup, 1);
	if (currentCup == data.end()) {
		return data.begin();
	}
	return currentCup;
}

void makeMoves(DataType& data, const int moves) {
	std::vector<DataType::iterator> iteratorRegistry(data.size() + 1);
	auto it = data.begin();
	for (const auto value : data) {
		iteratorRegistry[value] = it++;
	}

	const auto minValue = *std::min_element(data.begin(), data.end());
	const auto maxValue = *std::max_element(data.begin(), data.end());
	auto currentCup = data.begin();
	for (int move = 0; move < moves; ++move) {

		const auto firstIt = getNextCup(data, currentCup);
		const auto first = *firstIt;
		data.erase(firstIt);
		const auto secondIt = getNextCup(data, currentCup);
		const auto second = *secondIt;
		data.erase(secondIt);
		const auto thirdIt = getNextCup(data, currentCup);
		const auto third = *thirdIt;
		data.erase(thirdIt);

		auto destinationCupValue = *currentCup - 1;
		DataType::iterator destinationCup;
		while (true) {
			if (destinationCupValue < minValue) {
				destinationCupValue = maxValue;
			}
			if (destinationCupValue == first ||
				destinationCupValue == second ||
				destinationCupValue == third) {
				--destinationCupValue;
			} else {
				destinationCup = iteratorRegistry[destinationCupValue];
				break;
			}
		}

		std::advance(destinationCup, 1);
		auto last = data.insert(destinationCup, third);
		iteratorRegistry[*last] = last;
		last = data.insert(last, second);
		iteratorRegistry[*last] = last;
		last = data.insert(last, first);
		iteratorRegistry[*last] = last;
		currentCup = getNextCup(data, currentCup);
	}
}

void partOne(DataType data) {
	makeMoves(data, 100);

	auto newBegin = std::find(data.begin(), data.end(), 1);
	std::rotate(data.begin(), newBegin, data.end());
	data.erase(data.begin());

	std::stringstream stream;
	for (const auto value : data) {
		stream << value;
	}

	std::cout << "Part one: " << stream.str() << std::endl;
}

void partTwo(DataType data) {
	const auto startValue = *std::max_element(data.begin(), data.end()) + 1;
	for (int i = startValue; i <= 1000000; ++i) {
		data.push_back(i);
	}
	makeMoves(data, 10000000);

	const auto cupOne = std::find(data.begin(), data.end(), 1);
	const auto first = getNextCup(data, cupOne);
	const auto second = getNextCup(data, first);
	const int64_t result = ((int64_t)*first) * (*second);
	std::cout << "Part two: " << result << std::endl;
}

int main() {
	DataType data{ 5, 2, 3, 7, 6, 4, 8, 1, 9 };
	
	partOne(data);
	partTwo(data);
	return 0;
}