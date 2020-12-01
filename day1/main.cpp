#include <iostream>
#include <vector>
#include <tuple>
#include "../common/fileReader.h"

constexpr long EXPECTED_SUM = 2020;

std::pair<long, long> find2020Pair(const std::vector<long>& values) {
	const long valuesSize = values.size();
	for (long i = 0; i < valuesSize; ++i) {
		for (long j = i + 1; j < valuesSize; ++j) {
			const long val1 = values[i];
			const long val2 = values[j];
			if (val1 + val2 == EXPECTED_SUM) {
				return { val1, val2 };
			}
		}
	}
}

std::tuple<long, long, long> find2020Triplet(const std::vector<long>& values) {
	const long valuesSize = values.size();
	for (long i = 0; i < valuesSize; ++i) {
		for (long j = i + 1; j < valuesSize; ++j) {
			for (long k = j + 1; k < valuesSize; ++k) {
				const long val1 = values[i];
				const long val2 = values[j];
				const long val3 = values[k];
				if (val1 + val2 + val3 == EXPECTED_SUM) {
					return { val1, val2, val3 };
				}
			}
		}
	}
}

void partOne(const std::vector<long>& values) {
	const auto [val1, val2] = find2020Pair(values);
	std::cout << val1 << " * " << val2 << " = " << val1 * val2 << std::endl;
}

void partTwo(const std::vector<long>& values) {
	const auto [val1, val2, val3] = find2020Triplet(values);
	std::cout << val1 << " * " << val2  << " * " << val3 <<
		" = " << val1 * val2 * val3 << std::endl;
}

int main() {
	common::FileReader reader("input.txt");
	std::vector<long> values;
	for (std::string line; reader.nextLine(line);) {
		const long value = std::stol(line);
		values.push_back(value);
	}
	partOne(values);
	partTwo(values);
	return 0;
}