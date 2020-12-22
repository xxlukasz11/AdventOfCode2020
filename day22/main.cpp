#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <unordered_set>

struct Game {
	std::deque<int> deck1;
	std::deque<int> deck2;
};

using DataType = Game;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextLine(line) && !line.empty();) {
		if (line[0] != 'P') {
			data.deck1.push_back(std::stoi(line));
		}
	}
	for (std::string line; reader.nextLine(line) && !line.empty();) {
		if (line[0] != 'P') {
			data.deck2.push_back(std::stoi(line));
		}
	}
	return data;
}

int calculateScore(const std::deque<int>& deck) {
	int score = 0;
	int mul = deck.size();
	for (int i = 0; i < deck.size(); ++i) {
		score += deck[i] * mul;
		--mul;
	}
	return score;
}

void partOne(DataType data) {
	auto& deck1 = data.deck1;
	auto& deck2 = data.deck2;
	while (!deck1.empty() && !deck2.empty()) {
		const auto val1 = deck1.front();
		const auto val2 = deck2.front();
		deck1.pop_front();
		deck2.pop_front();
		if (val1 > val2) {
			deck1.push_back(val1);
			deck1.push_back(val2);
		}
		else {
			deck2.push_back(val2);
			deck2.push_back(val1);
		}
	}
	int score = 0;
	if (!deck1.empty()) {
		score = calculateScore(deck1);
	}
	else {
		score = calculateScore(deck2);
	}
	std::cout << "Part one: " << score << std::endl;
}

std::pair<bool, std::deque<int>> playRecursiveGame(std::deque<int> deck1, std::deque<int> deck2) {
	std::unordered_set<std::string> registry;
	while (!deck1.empty() && !deck2.empty()) {
		const std::string str1(deck1.begin(), deck1.end());
		const std::string str2(deck2.begin(), deck2.end());
		const std::string final = str1 + "_" + str2;
		if (registry.contains(final)) {
			return { true, deck1 };
		}
		registry.insert(final);

		const auto val1 = deck1.front();
		const auto val2 = deck2.front();
		deck1.pop_front();
		deck2.pop_front();

		if (val1 <= deck1.size() && val2 <= deck2.size()) {
			std::deque<int> newDeck1(deck1.begin(), deck1.begin() + val1);
			std::deque<int> newDeck2(deck2.begin(), deck2.begin() + val2);
			auto [player1Won, _] = playRecursiveGame(newDeck1, newDeck2);
			if (player1Won) {
				deck1.push_back(val1);
				deck1.push_back(val2);
			}
			else {
				deck2.push_back(val2);
				deck2.push_back(val1);
			}
		}
		else {
			if (val1 > val2) {
				deck1.push_back(val1);
				deck1.push_back(val2);
			}
			else {
				deck2.push_back(val2);
				deck2.push_back(val1);
			}
		}
	}
	if (deck2.empty()) {
		return { true, deck1 };
	}
	else {
		return { false, deck2 };
	}
}

void partTwo(const DataType& data) {
	const auto& deck1 = data.deck1;
	const auto& deck2 = data.deck2;
	auto [_, winningDeck] = playRecursiveGame(deck1, deck2);
	int score = calculateScore(winningDeck);
	std::cout << "Part two: " << score << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}