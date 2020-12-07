#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../common/fileReader.h"

static const char* GOLD_BAG = "shiny gold";

struct Bag {
	std::string color;
	int amount;
};

struct MainBag {
	std::string color;
	std::vector<Bag> bags;
};

MainBag parseBag(const std::string& str) {
	MainBag mainBag;
	std::istringstream stream(str);
	stream >> mainBag.color;
	for (std::string word; stream >> word;) {
		if (word != "bags") {
			mainBag.color += " " + word;
		}
		else {
			break;
		}
	}
	if (std::string word; stream >> word && word != "contain") {
		throw std::runtime_error("Contains not present");
	}
	for (std::string amount; stream >> amount;) {
		if (amount == "no") {
			break;
		}
		Bag bag;
		bag.amount = std::stoi(amount);
		stream >> bag.color;
		for (std::string word; stream >> word;) {
			if (word != "bags," && word != "bag," && word != "bags." && word != "bag.") {
				bag.color += " " + word;
			}
			else {
				break;
			}
		}
		mainBag.bags.push_back(bag);
	}
	
	return mainBag;
}

std::vector<MainBag> read() {
	std::vector<MainBag> bags;
	common::FileReader reader("input.txt");
	for (std::string line; reader.nextLine(line);) {
		if (!line.empty()) {
			bags.push_back(parseBag(line));
		}
	}
	return bags;
}

bool findGold(const std::vector<MainBag>& mainBags, const std::vector<Bag>& bags) {
	bool found = false;
	for (const auto& bag : bags) {
		if (std::find_if(bags.begin(), bags.end(), [](const Bag& bag) {
			return bag.color == GOLD_BAG;
			}) != bags.end()) {
			return true;
		}
	}
	for (const auto& bag : bags) {
		auto mainBag = std::find_if(mainBags.begin(), mainBags.end(), [&bag](const MainBag& b) {
			return b.color == bag.color;
			});
		found |= findGold(mainBags, mainBag->bags);
	}
	return found;
}

int countBags(const std::vector<MainBag>& mainBags, const std::vector<Bag>& bags) {
	int count = 0;
	for (const auto& bag : bags) {
		auto mainBag = std::find_if(mainBags.begin(), mainBags.end(), [&bag](const MainBag& b) {
			return b.color == bag.color;
		});
		count += bag.amount + bag.amount * countBags(mainBags, mainBag->bags);
	}
	return count;
}

void partOne(const std::vector<MainBag>& mainBags) {
	int colorCount = 0;
	for (const auto& mainBag : mainBags) {
		const auto& bags = mainBag.bags;
		if (findGold(mainBags, bags)) {
			++colorCount;
		}
	}
	std::cout << "Part one - colors: " << colorCount << std::endl;
}

void partTwo(const std::vector<MainBag>& mainBags) {
	auto goldBag = std::find_if(mainBags.begin(), mainBags.end(), [](const MainBag& b) {
		return b.color == GOLD_BAG;
	});
	const auto& bags = goldBag->bags;
	int bagCount = countBags(mainBags, bags);
	std::cout << "Part two - bags: " << bagCount << std::endl;
}

int main() {
	std::vector<MainBag> bags = read();
	partOne(bags);
	partTwo(bags);
	return 0;
}