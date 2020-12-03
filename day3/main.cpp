#include <iostream>
#include <vector>
#include <cstdint>
#include "../common/fileReader.h"

constexpr char TREE = '#';

int countTrees(const std::vector<std::string>& map, const int horizontalSlope, const int verticalSlope) {
	int treeCount = 0;;
	const int verticalSize = map.size();
	const int horizontalSize = map.at(0).size();
	for (int horizontal = 0, vertical = 0;
		vertical < verticalSize;
		vertical += verticalSlope, horizontal = (horizontal + horizontalSlope) % horizontalSize) {
		if (map[vertical][horizontal] == TREE) {
			++treeCount;
		}
	}
	return treeCount;
}

void partOne(const std::vector<std::string>& map) {
	const int treeCount = countTrees(map, 3, 1);
	std::cout << "Part one - number of trees: " << treeCount << std::endl;
}

void partTwo(const std::vector<std::string>& map) {
	const int slope1 = countTrees(map, 1, 1);
	const int slope2 = countTrees(map, 3, 1);
	const int slope3 = countTrees(map, 5, 1);
	const int slope4 = countTrees(map, 7, 1);
	const int slope5 = countTrees(map, 1, 2);
	std::cout << "Part two - multiplication: " << (int64_t)slope1 * slope2 * slope3 * slope4 * slope5 << std::endl;
}

int main() {
	common::FileReader reader("input.txt");
	std::vector<std::string> map;
	for (std::string line; reader.nextLine(line);) {
		map.push_back(line);
	}
	partOne(map);
	partTwo(map);
	return 0;
}