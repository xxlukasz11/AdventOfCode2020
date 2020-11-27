#include <iostream>
#include "../common/fileReader.h"

int main() {
	common::FileReader reader("input.txt");
	int totalFuel = 0;
	for (std::string line; reader.nextLine(line);) {
		const int value = std::stoi(line);
		const int fuel = value / 3 - 2;
		totalFuel += fuel;
	}
	std::cout << "Fuel: " << totalFuel << std::endl;
	return 0;
}