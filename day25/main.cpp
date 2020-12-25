#include "../common/fileReader.h"
#include <iostream>

struct PublicKeys {
	int key1;
	int key2;
};

using DataType = PublicKeys;

constexpr int DIVISOR = 20201227;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	data.key1 = std::stoi(reader.nextLine());
	data.key2 = std::stoi(reader.nextLine());
	return data;
}

void partOne(const DataType& data) {
	int loopSize = 1;
	const auto publicKey = data.key1;
	const int subjectNumber = 7;
	int64_t value = 1;
	while (true) {
		value *= subjectNumber;
		value %= DIVISOR;
		if (value == publicKey) {
			break;
		}
		++loopSize;
	}

	value = 1;
	const auto otherPublicKey = data.key2;
	for (int i = 0; i < loopSize; ++i) {
		value *= otherPublicKey;
		value %= DIVISOR;
	}

	std::cout << "Part one: " << value << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	return 0;
}