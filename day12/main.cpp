#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>

struct Instruction {
	char command;
	int value;
};

struct Waypoint {
	int horizontal;
	int vertical;
};

using DataType = std::vector<Instruction>;

static const std::unordered_map<char, std::pair<int, int>> DIRECTIONS{
		{'E', {1, 0}},
		{'W', {-1, 0}},
		{'N', {0, 1}},
		{'S', {0, -1}},
};

DataType read() {
	std::ifstream reader("input.txt");
	DataType data;
	for (char command; reader >> command;) {
		int value;
		reader >> value;
		data.push_back(Instruction{ command, value });
	}
	return data;
}

char findNewDirection(const char command, const int value, const char currentDirection) {
	if (value == 90) {
		if (command == 'L') {
			switch (currentDirection) {
				case 'N': return 'W';
				case 'W': return 'S';
				case 'S': return 'E';
				case 'E': return 'N';
			}
		} else if (command == 'R') {
			switch (currentDirection) {
				case 'N': return 'E';
				case 'W': return 'N';
				case 'S': return 'W';
				case 'E': return 'S';
			}
		} else {
			throw "wrong command";
		}
	}
	else if (value == 180) {
		switch (currentDirection) {
			case 'N': return 'S';
			case 'W': return 'E';
			case 'S': return 'N';
			case 'E': return 'W';
			default: throw "wrong command";
		}
	}
	else if (value == 270) {
		if (command == 'R') {
			return findNewDirection('L', 90, currentDirection);
		} else {
			return findNewDirection('R', 90, currentDirection);
		}
	} else {
		throw "wrong command";
	}
}

void partOne(const DataType& data) {
	auto currentDirection = 'E';
	auto currentVector = DIRECTIONS.find(currentDirection)->second;
	int currentX = 0;
	int currentY = 0;
	for (const auto [command, value] : data) {
		if (command == 'F') {
			currentX += currentVector.first*value;
			currentY += currentVector.second*value;
		} else if (command == 'N' || command == 'E' || command == 'W' || command == 'S') {
			auto tmpDir = DIRECTIONS.find(command)->second;
			currentX += tmpDir.first * value;
			currentY += tmpDir.second * value;;
		} else {
			currentDirection = findNewDirection(command, value, currentDirection);
			currentVector = DIRECTIONS.find(currentDirection)->second;
		}
	}
	std::cout << "Part one: " << abs(currentX) + abs(currentY) << std::endl;
}

Waypoint rotateWaypoint(const Waypoint waypoint, const char command, const int value) {
	if (value == 180) {
		return {-waypoint.horizontal, -waypoint.vertical};
	}
	if (value == 90) {
		if (command == 'L') {
			return { -waypoint.vertical, waypoint.horizontal };
		} else {
			return { waypoint.vertical, -waypoint.horizontal };
		}
	}
	if (value == 270) {
		if (command == 'L') {
			return { waypoint.vertical, -waypoint.horizontal };
		} else {
			return { -waypoint.vertical, waypoint.horizontal };
		}
	}
	throw "wrong command";
}

void partTwo(const DataType& data) {
	Waypoint waypoint{ 10, 1 };
	int currentX = 0;
	int currentY = 0;
	for (const auto [command, value] : data) {
		if (command == 'F') {
			currentX += waypoint.horizontal * value;
			currentY += waypoint.vertical * value;
		} else if (command == 'N' || command == 'E' || command == 'W' || command == 'S') {
			auto dir = DIRECTIONS.find(command)->second;
			if (command == 'N' || command == 'S') {
				waypoint.vertical += dir.second * value;
			} else {
				waypoint.horizontal += dir.first * value;
			}
		} else {
			waypoint = rotateWaypoint(waypoint, command, value);
		}
	}
	std::cout << "Part two: " << abs(currentX) + abs(currentY) << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}