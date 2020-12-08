#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "../common/fileReader.h"

enum class Keyword {
	JMP,
	NOP,
	ACC
};

struct Command {
	Keyword keyword;
	int value;
};

Keyword decodeKeyword(const std::string& str) {
	if (str == "jmp") {
		return Keyword::JMP;
	}
	if (str == "acc") {
		return Keyword::ACC;
	}
	if (str == "nop") {
		return Keyword::NOP;
	}
	throw std::runtime_error("Wrong keyword");
}

std::vector<Command> read() {
	std::vector<Command> commands;
	common::FileReader reader("input.txt");
	for (std::string line; reader.nextLine(line);) {
		const auto space = line.find(' ');
		const auto keyword = decodeKeyword(line.substr(0, space));
		const auto sign = line.substr(space + 1, 1);
		const auto value = std::stoi(line.substr(space + 2));
		Command command{ keyword, sign == "+" ? value : -value };
		commands.push_back(command);
	}
	return commands;
}

std::pair<bool, int> executeProgram(const std::vector<Command> commands) {
	int accumulator = 0;
	int currentLine = 0;
	std::unordered_set<int> executedLines;
	const auto noOfCommands = commands.size();
	for (; currentLine < noOfCommands;) {
		const auto [keyword, value] = commands[currentLine];
		const bool wasExecuted = executedLines.find(currentLine) != executedLines.end();
		if (wasExecuted) {
			break;
		} else {
			executedLines.insert(currentLine);
		}
		if (keyword == Keyword::ACC) {
			accumulator += value;
			++currentLine;
		} else if (keyword == Keyword::JMP) {
			currentLine += value;
		} else {
			++currentLine;
		}
	}
	return { currentLine == noOfCommands, accumulator };
}

void partOne(const std::vector<Command>& commands) {
	auto [_, accumulator] = executeProgram(commands);
	std::cout << "Part one: accumulator = " << accumulator << std::endl;
}

void partTwo(const std::vector<Command>& commands) {
	int accumulator = 0;
	for (int i = 0; i < commands.size(); ++i) {
		if (commands[i].keyword == Keyword::JMP) {
			auto newCommands = commands;
			newCommands[i].keyword = Keyword::NOP;
			const auto [last, acc] = executeProgram(newCommands);
			if (last) {
				accumulator = acc;
				break;
			}
		}
		if (commands[i].keyword == Keyword::NOP) {
			auto newCommands = commands;
			newCommands[i].keyword = Keyword::JMP;
			const auto [last, acc] = executeProgram(newCommands);
			if (last) {
				accumulator = acc;
				break;
			}
		}
	}
	std::cout << "Part two: accumulator = " << accumulator << std::endl;
}

int main() {
	const std::vector<Command> commands = read();
	partOne(commands);
	partTwo(commands);
	return 0;
}