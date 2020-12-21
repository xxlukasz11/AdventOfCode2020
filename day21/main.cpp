#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>

struct Food {
	std::vector<std::string> ingredients;
	std::vector<std::string> allergens;
};

using DataType = std::vector<Food>;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextLine(line);) {
		Food food;
		std::istringstream stream(line);
		for (std::string word; stream >> word && word[0] != '(';) {
			food.ingredients.push_back(word);
		}
		for (std::string word; stream >> word;) {
			word.pop_back();
			food.allergens.push_back(word);
		}
		data.push_back(food);
	}
	return data;
}

void playground(const DataType& data) {
	std::set<std::string> allAllergens;
	for (const auto& [_, allergens] : data) {
		allAllergens.insert(allergens.begin(), allergens.end());
	}
	std::map<std::string, std::vector<std::string>> goodIngredients;
	for (const auto& [ing, all] : data) {
		std::vector<std::string> notPresentAllergens(allAllergens.begin(), allAllergens.end());
		for (const auto& allergen : all) {
			auto found = std::find(notPresentAllergens.begin(), notPresentAllergens.end(), allergen);
			notPresentAllergens.erase(found);
		}
		for (const auto& notPresent : notPresentAllergens) {
			auto& array = goodIngredients[notPresent];
			array.insert(array.end(), ing.begin(), ing.end());
		}
	}

	std::vector<std::vector<std::string>> toIntersect;
	for (auto& mapEntry : goodIngredients) {
		toIntersect.push_back(std::move(mapEntry.second));
		std::sort(toIntersect.back().begin(), toIntersect.back().end());
		std::unique(toIntersect.back().begin(), toIntersect.back().end());
	}

	std::vector<std::string> validIngredients;
	for (int i = 0; i < toIntersect.size(); ++i) {
		for (const auto& ingredient : toIntersect[i]) {
			bool valid = true;
			for (int j = 0; j < toIntersect.size(); ++j) {
				if (i != j) {
					if (std::find(toIntersect[j].begin(), toIntersect[j].end(), ingredient) != toIntersect[j].end()) {
						valid = false;
						break;
					}
				}
			}
			if (valid) {
				validIngredients.push_back(ingredient);
			}
		}

	}
}

std::vector<std::string> determineAllergenFreeIngredients(const DataType& data) {
	// map: ingredient -> all possible allergens
	std::map<std::string, std::vector<std::string>> possibleAllergens;
	for (const auto& [ingredients, allergens] : data) {
		for (const auto& ing : ingredients) {
			auto& array = possibleAllergens[ing];
			array.insert(array.end(), allergens.begin(), allergens.end());
		}
	}

	// remove duplicates from allergen list
	for (auto& entry : possibleAllergens) {
		auto& array = entry.second;
		std::sort(array.begin(), array.end());
		auto last = std::unique(array.begin(), array.end());
		array.erase(last, array.end());
	}

	// copy to safely erase allergens
	auto pool = possibleAllergens;

	// if food contains given allergen, but does not contain given ingredient, then the ingredient cannot contain the allergen
	for (const auto& entry : possibleAllergens) {
		const auto& ingName = entry.first;
		const auto& array = entry.second;
		for (const auto& allergen : array) {
			for (const auto& food : data) {
				if (std::find(food.allergens.begin(), food.allergens.end(), allergen) != food.allergens.end()) {
					if (std::find(food.ingredients.begin(), food.ingredients.end(), ingName) == food.ingredients.end()) {
						auto& arr = pool[ingName];
						arr.erase(std::remove(arr.begin(), arr.end(), allergen), arr.end());
						break;
					}
				}
			}
		}
	}

	// find ingredients with no possible allergens left
	std::vector<std::string> allergenFreeIngredients;
	for (const auto& entry : pool) {
		if (entry.second.empty()) {
			allergenFreeIngredients.push_back(entry.first);
		}
	}
	return allergenFreeIngredients;
}

void partOne(const DataType& data) {
	std::vector<std::string> allergenFreeIngredients = determineAllergenFreeIngredients(data);
	int count = 0;
	for (const auto& ingName : allergenFreeIngredients) {
		for (const auto& [ingredients, _] : data) {
			if (std::find(ingredients.begin(), ingredients.end(), ingName) != ingredients.end()) {
				++count;
			}
		}
	}
	std::cout << "Part one: " << count << std::endl;
}

void partTwo(const DataType& data) {
	std::vector<std::string> allergenFreeIngredients = determineAllergenFreeIngredients(data);

	// find ingredients that can contain allergens
	std::set<std::string> canonicalIngredients;
	for (const auto& [ingArray, _] : data) {
		for (const auto& ingName : ingArray) {
			if (std::find(allergenFreeIngredients.begin(), allergenFreeIngredients.end(), ingName) == allergenFreeIngredients.end()) {
				canonicalIngredients.insert(ingName);
			}
		}
	}

	// set of all possible allergens
	std::set<std::string> allAllergens;
	for (const auto& [_, allergens] : data) {
		allAllergens.insert(allergens.begin(), allergens.end());
	}

	// prepare map: canonical ingredient -> possible allergens
	std::map<std::string, std::vector<std::string>> possibleAllergens;
	for (const auto& ingName : canonicalIngredients) {
		possibleAllergens.emplace(ingName, std::vector<std::string>(allAllergens.begin(), allAllergens.end()));
	}

	// copy to safely remove allergens
	auto pool = possibleAllergens;

	// if food contains given allergen, but does not contain given ingredient, then the ingredient cannot contain the allergen
	for (const auto& entry : possibleAllergens) {
		const auto& ingName = entry.first;
		const auto& array = entry.second;
		for (const auto& allergen : array) {
			for (const auto& food : data) {
				if (std::find(food.allergens.begin(), food.allergens.end(), allergen) != food.allergens.end()) {
					if (std::find(food.ingredients.begin(), food.ingredients.end(), ingName) == food.ingredients.end()) {
						auto& arr = pool[ingName];
						arr.erase(std::remove(arr.begin(), arr.end(), allergen), arr.end());
						break;
					}
				}
			}
		}
	}

	// if ingredient has only one possible allergen, then remove this allergen from other ingredients
	// in eahc iteration, there always will be an ingredient with only one possible allergen
	std::vector<std::pair<std::string, std::string>> ingredientToAllergen;
	while (ingredientToAllergen.size() < possibleAllergens.size()) {
		auto ingredientWithOneAllergen = std::find_if(pool.begin(), pool.end(), [](const auto& entry) {
			return entry.second.size() == 1;
		});
		auto allergen = ingredientWithOneAllergen->second.front();
		ingredientToAllergen.emplace_back(ingredientWithOneAllergen->first, allergen);

		pool.erase(ingredientWithOneAllergen);
		for (auto& entry : pool) {
			auto allergenToRemove = std::find(entry.second.begin(), entry.second.end(), allergen);
			if (allergenToRemove != entry.second.end()) {
				entry.second.erase(allergenToRemove);
			}
		}
	}

	// sort by allergen
	std::sort(ingredientToAllergen.begin(), ingredientToAllergen.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.second < rhs.second;
	});

	// create canonical list
	std::string canonicalList;
	for (const auto& [ingName, _] : ingredientToAllergen) {
		canonicalList += ingName + ',';
	}
	canonicalList.pop_back();

	std::cout << "Part two: " << canonicalList << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}