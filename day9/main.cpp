#include <iostream>
#include <string>
#include <vector>
#include "../common/fileReader.h"
#include <numeric>
#include <algorithm>

constexpr long long NUMBER = 373803594;

std::vector<long long> read() {
	common::FileReader reader("input.txt");
	std::vector<long long> values;
	for (std::string line; reader.nextWord(line);) {
		const auto value = std::stoll(line);
		values.push_back(value);
	}
	return values;
}

template<typename T>
bool isValid(const T start, const T end, long long value) {
	for (auto first = start; first != end; first++) {
		for (auto second = first + 1; second != end; second++) {
			if (*second + *first == value) {
				return true;
			}
		}
	}
	return false;
}

void partOne(const std::vector<long long>& values) {
	long long value = 0;
	auto start = values.begin();
	auto end = start + 25;
	while (end != values.end()) {
		const auto val = *end;
		if (!isValid(start, end, val)) {
			value = val;
			break;
		}
		start++;
		end++;
	}
	std::cout << "Part one - value: " << value << std::endl;
}

void partTwo(const std::vector<long long>& values) {
	auto widestStart = values.begin();
	auto widestEnd = values.begin() + 1;
	int maxWidth = 0;
	for (auto first = values.begin(); first != values.end() - 1; ++first) {
		for (auto second = first + 1; second != values.end(); ++second) {
			const auto sum = std::accumulate(first, second, 0LL);
			if (sum > NUMBER) {
				break;
			} else if (NUMBER == sum) {
				int width = second - first + 1;
				if (width > maxWidth) {
					maxWidth = width;
					widestStart = first;
					widestEnd = second;
				}
				break;
			}
		}
	}
	const auto maxSum = *std::min_element(widestStart, widestEnd) + *std::max_element(widestStart, widestEnd);
	std::cout << "Part two - value: " << maxSum << std::endl;
}

int main() {
	const auto values = read();
	partOne(values);
	partTwo(values);
	return 0;
}