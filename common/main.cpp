#include <iostream>
#include "fileReader.h"

int main() {
	FileReader reader("dummy.txt");
	std::cout << reader.nextWord() << std::endl;
	for (std::string line; reader.nextLine(line);) {
		std::cout << line << std::endl;
	}
	return 0;
}