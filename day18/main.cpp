#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using DataType = std::vector<std::vector<char>>;

enum class Operation {
	MULTIPLY,
	ADD,
	NONE
};

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextLine(line);) {
		std::istringstream stream(line);
		std::vector<char> row;
		for (char c; stream >> c;) {
			row.push_back(c);
		}
		data.push_back(row);
	}
	return data;
}

int64_t toDigit(char c) {
	return c - '0';
}

int64_t compute(const int64_t left, const int64_t right, const Operation operation) {
	switch (operation) {
		case Operation::ADD: return left + right;
		case Operation::MULTIPLY: return left * right;
		default: throw "Unsupported operation";
	}
}

int64_t seekToScopeEnd(const std::vector<char>& expression, const int64_t startIndex) {
	int64_t braceIndicator = 0;
	for (int64_t i = startIndex; i < expression.size(); ++i) {
		if (expression[i] == '(') {
			++braceIndicator;
		}
		else if (expression[i] == ')') {
			--braceIndicator;
		}
		if (braceIndicator == 0) {
			return i;
		}
	}
	throw "Cannot find scope end";
}

int64_t evaluateNoPrecedence(const std::vector<char>& expression, const int64_t startIndex, const int64_t stopIndex) {
	int64_t currentValue = 0;
	Operation operation = Operation::NONE;
	for (int64_t i = startIndex; i < stopIndex; ++i) {
		const char symbol = expression[i];
		if (symbol == '(') {
			int64_t end = seekToScopeEnd(expression, i);
			int64_t valueFromParentheses = evaluateNoPrecedence(expression, i + 1, end);
			if (operation != Operation::NONE) {
				currentValue = compute(currentValue, valueFromParentheses, operation);
			}
			else {
				currentValue = valueFromParentheses;
			}
			i = end;
		}
		else if (symbol == ')') {
			// do nothing
		}
		else if (symbol == '+') {
			operation = Operation::ADD;
		}
		else if (symbol == '*') {
			operation = Operation::MULTIPLY;
		}
		else {
			if (operation == Operation::NONE) {
				currentValue = toDigit(symbol);
			}
			else {
				currentValue = compute(currentValue, toDigit(symbol), operation);
			}
		}
	}
	return currentValue;
}

int64_t computeWithPrecedence(std::vector<int64_t> values, std::vector<Operation> operations) {
	if (values.empty()) {
		return 0;
	}

	auto addition = std::find(operations.begin(), operations.end(), Operation::ADD);
	while (addition != operations.end()) {
		const int operationIndex = std::distance(operations.begin(), addition);
		const auto firstValueIndex = operationIndex;
		const auto secondValueIndex = operationIndex + 1;
		const auto sum = values[firstValueIndex] + values[secondValueIndex];
		values[secondValueIndex] = sum;
		values.erase(values.begin() + firstValueIndex);
		operations.erase(operations.begin() + operationIndex);
		addition = std::find(operations.begin(), operations.end(), Operation::ADD);
	}

	int64_t mul = 1;
	for (const auto value : values) {
		mul *= value;
	}
	return mul;
}

int64_t evaluateWithPrecedence(const std::vector<char>& expression, const int64_t startIndex, const int64_t stopIndex) {
	std::vector<int64_t> values;
	std::vector<Operation> operations;
	for (int64_t i = startIndex; i < stopIndex; ++i) {
		const char symbol = expression[i];
		if (symbol == '(') {
			int64_t end = seekToScopeEnd(expression, i);
			int64_t valueFromParentheses = evaluateWithPrecedence(expression, i + 1, end);
			values.push_back(valueFromParentheses);
			i = end;
		}
		else if (symbol == ')') {
			// do nothing
		}
		else if (symbol == '+') {
			operations.push_back(Operation::ADD);
		}
		else if (symbol == '*') {
			operations.push_back(Operation::MULTIPLY);
		}
		else {
			values.push_back(toDigit(symbol));
		}
	}
	return computeWithPrecedence(values, operations);
}

void partOne(const DataType& data) {
	int64_t sum = 0;
	for (const auto& row : data) {
		sum += evaluateNoPrecedence(row, 0, row.size());
	}
	std::cout << "Part one: " << sum << std::endl;
}

void partTwo(const DataType& data) {
	int64_t sum = 0;
	for (const auto& row : data) {
		sum += evaluateWithPrecedence(row, 0, row.size());
	}
	std::cout << "Part two: " << sum << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}