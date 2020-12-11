#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>

using DataType = std::vector<int>;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextLine(line);) {

	}
	return data;
}

void partOne(const DataType& data) {

	std::cout << "Part one: " << std::endl;
}

void partTwo(const DataType& data) {

	std::cout << "Part two: " << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}