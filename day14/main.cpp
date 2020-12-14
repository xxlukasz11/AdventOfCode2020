#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

struct MemoryWrite {
	uint64_t address;
	uint64_t value;
};

struct Instruction {
	std::string mask;
	std::vector<MemoryWrite> writings;
};

using DataType = std::vector<Instruction>;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	reader.nextWord();
	for (std::string line; reader.nextWord(line);) {
		Instruction instruction;
		instruction.mask = reader.nextWord();
		for (std::string memory; reader.nextWord(memory) && memory != "mask";) {
			auto begin = memory.find("[");
			auto end = memory.find("]");
			uint64_t address = std::stoull(memory.substr(begin + 1, end - begin - 1));
			reader.nextWord();
			uint64_t value = std::stoull(reader.nextWord());
			instruction.writings.push_back(MemoryWrite{ address, value });
		}
		data.push_back(instruction);
	}
	return data;
}

uint64_t getZeores(const std::string& mask) {
	uint64_t zeroes = 0;
	int index = mask.size() - 1;
	bool oneAppeared = false;
	for (const char c : mask) {
		if (c == '0') {
			zeroes &= ~(1ULL << index);
		}
		else if (c == '1') {
			oneAppeared = true;
			zeroes |= (1ULL << index);
		}
		else if(oneAppeared) {
			zeroes |= (1ULL << index);
		}
		--index;
	}
	return zeroes;
}

uint64_t getOnes(const std::string& mask) {
	uint64_t ones = 0;
	int index = mask.size() - 1;
	for (const char c : mask) {
		if (c == '1') {
			ones |= (1ULL << index);
		}
		--index;
	}
	return ones;
}

void partOne(const DataType& data) {
	std::unordered_map<uint64_t, uint64_t> registry;
	for (const auto& [mask, writings] : data) {
		const auto zeroes = getZeores(mask);
		const auto ones = getOnes(mask);
		for (const auto& [address, value] : writings) {
			registry[address] = (value | ones) & zeroes;
		}
	}
	uint64_t sum = 0;
	for (const auto& entry : registry) {
		sum += entry.second;
	}
	std::cout << "Part one: " << sum << std::endl;
}

std::vector<int> getPositionOfX(const std::string& mask) {
	int index = mask.size() - 1;
	std::vector<int> exes;
	for (const char c : mask) {
		if (c == 'X') {
			exes.push_back(index);
		}
		--index;
	}
	return exes;
}

std::vector<uint64_t> allPossible(const uint64_t address, std::vector<int> exes) {
	if (exes.empty()) {
		return {address};
	}
	const auto xPos = exes.back();
	exes.pop_back();
	const auto addrOne = address | (1ULL << xPos);
	std::vector<uint64_t> addressesZero = allPossible(address, exes);
	std::vector<uint64_t> addressesOne = allPossible(addrOne, exes);
	addressesZero.insert(addressesZero.end(), addressesOne.begin(), addressesOne.end());
	return addressesZero;
}

std::vector<uint64_t> generateAddresses(const uint64_t address, const std::vector<int>& exes) {
	auto baseAddress = address;
	for (const auto xPos : exes) {
		baseAddress &= ~(1ULL << xPos);
	}
	return allPossible(baseAddress, exes);
}

void partTwo(const DataType& data) {
	std::unordered_map<uint64_t, uint64_t> registry;
	for (const auto& [mask, writings] : data) {
		const auto ones = getOnes(mask);
		const auto exes = getPositionOfX(mask);
		for (const auto& [baseAddress, value] : writings) {
			const auto newBaseAddress = baseAddress | ones;
			const auto allAddresses = generateAddresses(newBaseAddress, exes);
			for (const auto addr : allAddresses) {
				registry[addr] = value;
			}
		}
	}
	uint64_t sum = 0;
	for (const auto& entry : registry) {
		sum += entry.second;
	}
	std::cout << "Part two: " << sum << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}