#include "../common/fileReader.h"
#include "../common/Vec3.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include <deque>

enum class Instruction {
	WEST,
	NORTH_WEST,
	SOUTH_WEST,
	EAST,
	NORTH_EAST,
	SOUTH_EAST
};

static const std::unordered_map<Instruction, std::pair<int, int>> DIRECTIONS{
	{Instruction::WEST, {-2, 0}},
	{Instruction::NORTH_WEST, {-1, -1}},
	{Instruction::SOUTH_WEST, {-1, 1}},
	{Instruction::EAST, {2, 0}},
	{Instruction::NORTH_EAST, {1, -1}},
	{Instruction::SOUTH_EAST, {1, 1}},
};

constexpr int WHITE = 0;
constexpr int BLACK = 1;

using DataType = std::vector<std::vector<Instruction>>;
using common::Vec3;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextLine(line);) {
		std::vector<Instruction> instructions;
		std::istringstream stream(line);
		for (char c; stream >> c;) {
			Instruction instruction;
			if (c == 'e') {
				instruction = Instruction::EAST;
			}
			else if (c == 'w') {
				instruction = Instruction::WEST;
			}
			else if (c == 's') {
				char next;
				stream >> next;
				if (next == 'e') {
					instruction = Instruction::SOUTH_EAST;
				} else {
					instruction = Instruction::SOUTH_WEST;
				}
			}
			else if (c == 'n') {
				char next;
				stream >> next;
				if (next == 'e') {
					instruction = Instruction::NORTH_EAST;
				} else {
					instruction = Instruction::NORTH_WEST;
				}
			}
			instructions.push_back(instruction);
		}
		data.push_back(instructions);
	}
	return data;
}

std::map<Vec3<int>, bool> generateBlackTilesMap(const DataType& data) {
	std::map<Vec3<int>, bool> isBlack;
	for (const auto& intructions : data) {
		int x = 0;
		int y = 0;
		for (const auto& dir : intructions) {
			const auto& vec = DIRECTIONS.find(dir)->second;
			x += vec.first;
			y += vec.second;
		}
		Vec3<int> pos{ x, y, 0 };
		auto& value = isBlack[pos];
		value = !value;
	}
	return isBlack;
}

void partOne(const DataType& data) {
	std::map<Vec3<int>, bool> isBlack = generateBlackTilesMap(data);
	int count = 0;
	for (const auto& entry : isBlack) {
		if (entry.second == true) {
			++count;
		}
	}
	std::cout << "Part one: " << count << std::endl;
}

int countBlackNeighbours(const std::deque<std::deque<int>>& map, const int x, const int y) {
	int count = 0;
	for (const auto& entry : DIRECTIONS) {
		const auto& vec = entry.second;
		const int nX = x + vec.first;
		const int nY = y + vec.second;
		if (map[nY][nX] == BLACK) {
			++count;
		}
	}
	return count;
}

bool isPartOfHexagonalGrid(const int x, const int y, const int xCenter, const int yCenter) {
	if (y % 2 == yCenter % 2 && x % 2 != xCenter % 2) {
		return false;
	} else if (y % 2 != yCenter % 2 && x % 2 == xCenter % 2) {
		return false;
	}
	return true;
}

void partTwo(const DataType& data) {
	std::map<Vec3<int>, bool> isBlack = generateBlackTilesMap(data);

	int maxX = 0;
	int maxY = 0;
	for (const auto& entry : isBlack) {
		if (abs(entry.first.x) > maxX) {
			maxX = abs(entry.first.x);
		}
		if (abs(entry.first.y) > maxY) {
			maxY = abs(entry.first.y);
		}
	}
	
	const int mapXSize = (maxX + 4)*2 + 1;
	const int mapYSize = (maxY + 2)*2 + 1;
	int xCenter = mapXSize / 2;
	int yCenter = mapYSize / 2;

	// fill initial map
	std::deque<std::deque<int>> map(mapYSize, std::deque<int>(mapXSize, WHITE));
	for (const auto& intructions : data) {
		int x = xCenter;
		int y = yCenter;
		for (const auto& dir : intructions) {
			const auto& vec = DIRECTIONS.find(dir)->second;
			x += vec.first;
			y += vec.second;
		}
		auto& value = map[y][x];
		if (value == WHITE) {
			value = BLACK;
		} else if (value == BLACK) {
			value = WHITE;
		}
	}

	// simulate flips
	for (int day = 0; day < 100; ++day) {
		auto newMap = map;
		const int ySize = map.size();
		const int xSize = map.front().size();
		for (int y = 1; y < ySize - 1; ++y) {
			for (int x = 2; x < xSize - 2; ++x) {
				if (!isPartOfHexagonalGrid(x, y, xCenter, yCenter)) {
					continue;
				}
				const int blackNeighbours = countBlackNeighbours(map, x, y);
				const auto& value = map[y][x];
				if (value == BLACK && (blackNeighbours == 0 || blackNeighbours > 2)) {
					newMap[y][x] = WHITE;
				} else if (value == WHITE && blackNeighbours == 2) {
					newMap[y][x] = BLACK;
				}
			}
		}
		map = newMap;
		
		// extend map
		for (auto& row : map) {
			row.push_front(WHITE);
			row.push_front(WHITE);
			row.push_back(WHITE);
			row.push_back(WHITE);
		}
		map.push_front(std::deque<int>(map.front().size(), WHITE));
		map.push_back(std::deque<int>(map.front().size(), WHITE));
		xCenter += 2;
		yCenter += 1;
	}

	int count = 0;
	for (const auto& row : map) {
		for (const auto& value : row) {
			if (value == BLACK) {
				++count;
			}
		}
	}
	std::cout << "Part two: " << count << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}