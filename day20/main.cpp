#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <deque>

//#include <sstream>
#include <algorithm>
//#include <numeric>
#include <unordered_map>
#include <map>

struct Tile {
	int id;
	std::string top;
	std::string left;
	std::string right;
	std::string bottom;
	std::deque<std::deque<char>> map;
};

using DataType = std::vector<Tile>;

enum BorderType {
	NONE,
	TOP,
	LEFT,
	RIGHT,
	BOTTOM,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

DataType read() {
	common::FileReader reader("test.txt");
	DataType data;
	for (std::string line; reader.nextLine(line);) {
		Tile tile;
		auto space = line.find(' ');
		auto colon = line.find(':');
		tile.id = std::stoi(line.substr(space + 1, colon - space - 1));

		line = reader.nextLine();
		tile.top = line;
		tile.left = line.front();
		tile.right = line.back();
		tile.map.push_back(std::deque(line.begin(), line.end()));
		for (std::string row; reader.nextLine(row) && !row.empty();) {
			tile.left += row.front();
			tile.right += row.back();
			tile.bottom = row;
			tile.map.push_back(std::deque(row.begin(), row.end()));
		}
		data.push_back(tile);
	}
	return data;
}

void flipVertical(Tile& tile) {
	std::reverse(tile.left.begin(), tile.left.end());
	std::reverse(tile.right.begin(), tile.right.end());
	std::swap(tile.top, tile.bottom);
}

void flipHorizontal(Tile& tile) {
	std::reverse(tile.top.begin(), tile.top.end());
	std::reverse(tile.bottom.begin(), tile.bottom.end());
	std::swap(tile.left, tile.right);
}

void rotate180(Tile& tile) {
	flipVertical(tile);
	flipHorizontal(tile);
}

void rotate90(Tile& tile) {
	std::swap(tile.top, tile.right);
	std::swap(tile.bottom, tile.top);
	std::swap(tile.left, tile.top);
	std::reverse(tile.bottom.begin(), tile.bottom.end());
	std::reverse(tile.top.begin(), tile.top.end());
}

void flipVertical(DataType& data, int id) {
	auto tile = std::find_if(data.begin(), data.end(), [id](const auto& tile) {
		return tile.id == id;
		});
	flipVertical(*tile);
}

void flipHorizontal(DataType& data, int id) {
	auto tile = std::find_if(data.begin(), data.end(), [id](const auto& tile) {
		return tile.id == id;
		});
	flipHorizontal(*tile);
}

void rotate90(DataType& data, int id) {
	auto tile = std::find_if(data.begin(), data.end(), [id](const auto& tile) {
		return tile.id == id;
		});
	rotate90(*tile);
}

BorderType lookForBorder(std::vector<Tile*> pool, int i) {
	auto center = *(pool[i]);
	pool.erase(pool.begin() + i);
	bool topFree = true;
	bool leftFree = true;
	bool rightFree = true;
	bool bottomFree = true;
	for (auto tile : pool) {
		if (center.top == tile->top || center.top == tile->bottom || center.top == tile->left || center.top == tile->right) {
			topFree = false;
		}
		if (center.bottom == tile->top || center.bottom == tile->bottom || center.bottom == tile->left || center.bottom == tile->right) {
			bottomFree = false;
		}
		if (center.right == tile->top || center.right == tile->bottom || center.right == tile->left || center.right == tile->right) {
			rightFree = false;
		}
		if (center.left == tile->top || center.left == tile->bottom || center.left == tile->left || center.left == tile->right) {
			leftFree = false;
		}
		flipVertical(center);
		if (center.right == tile->top || center.right == tile->bottom || center.right == tile->left || center.right == tile->right) {
			rightFree = false;
		}
		if (center.left == tile->top || center.left == tile->bottom || center.left == tile->left || center.left == tile->right) {
			leftFree = false;
		}
		flipVertical(center);
		flipHorizontal(center);
		if (center.top == tile->top || center.top == tile->bottom || center.top == tile->left || center.top == tile->right) {
			topFree = false;
		}
		if (center.bottom == tile->top || center.bottom == tile->bottom || center.bottom == tile->left || center.bottom == tile->right) {
			bottomFree = false;
		}
		flipHorizontal(center);
	}

	if (bottomFree && rightFree) {
		return BOTTOM_RIGHT;
	}
	if (bottomFree && leftFree) {
		return BOTTOM_LEFT;
	}
	if (topFree && leftFree) {
		return TOP_LEFT;
	}
	if (topFree && rightFree) {
		return TOP_RIGHT;
	}
	if (bottomFree) {
		return BOTTOM;
	}
	if (topFree) {
		return TOP;
	}
	if (leftFree == 0) {
		return LEFT;
	}
	if (rightFree) {
		return RIGHT;
	}
	return NONE;
}

void partOne(DataType data) {
	std::vector<Tile*> pool;
	for (auto& tile : data) {
		pool.push_back(&tile);
	}

	int64_t mul = 1;
	for (int i = 0; i < data.size(); ++i) {
		auto border = lookForBorder(pool, i);
		if (border == TOP_LEFT || border == TOP_RIGHT || border == BOTTOM_LEFT || border == BOTTOM_RIGHT) {
			mul *= data[i].id;
		}
	}
	std::cout << "Part one: " << mul << std::endl;
}

int findMatchingLeft(const std::string border, std::vector<Tile*>& tiles) {
	for (int i = 0; i < tiles.size(); ++i) {
		auto& tile = tiles[i];
		if (border == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->left) {
			return i;
		}
		flipHorizontal(*tile);
		if (border == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->left) {
			return i;
		}
	}
	return -1;
}

int findMatchingTop(const std::string border, std::vector<Tile*>& tiles) {
	for (int i = 0; i < tiles.size(); ++i) {
		auto tile = tiles[i];
		if (border == tile->top) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->top) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->top) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->top) {
			return i;
		}
		flipHorizontal(*tile);
		if (border == tile->top) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->top) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->top) {
			return i;
		}
		rotate90(*tile);
		if (border == tile->top) {
			return i;
		}
	}
	return -1;
}

bool findSeaMonster(std::vector<std::vector<char>>& seaMap, int x, int y) {
	static const std::vector<std::pair<int, int>> directives{
		{1,1},
		{3,0},
		{1,-1},
		{1,0},
		{1,1},
		{3,0},
		{1,-1},
		{1,0},
		{1,1},
		{3,0},
		{1,-1},
		{1,-1},
		{0,1},
		{1,0}
	};
	const int seaSize = seaMap.size();
	y += 1;
	int directiveIndex = -1;
	auto newSea = seaMap;
	while (x >= 0 && y >= 0 && x < seaSize && y < seaSize) {
		if (seaMap[y][x] != '#' && seaMap[y][x] != 'o') {
			return false;
		}
		newSea[y][x] = 'o';
		++directiveIndex;
		if (directiveIndex == directives.size()) {
			seaMap = newSea;
			return true;
		}
		x += directives[directiveIndex].first;
		y += directives[directiveIndex].second;
	}
	return false;
}

void rotateMatrix(std::vector<std::vector<char>>& matrix) {
	const int size = matrix.size();
	for (int x = 0; x < size / 2; x++) {
		for (int y = x; y < size - x - 1; y++) {
			int temp = matrix[x][y];
			matrix[x][y] = matrix[y][size - 1 - x];
			matrix[y][size - 1 - x] = matrix[size - 1 - x][size - 1 - y];
			matrix[size - 1 - x][size - 1 - y] = matrix[size - 1 - y][x];
			matrix[size - 1 - y][x] = temp;
		}
	}
}

void partTwo(DataType data) {
	const int tilesInRow = sqrt(data.size());
	std::vector<std::vector<Tile*>> grid(tilesInRow, std::vector<Tile*>(tilesInRow, nullptr));
	std::vector<Tile*> pool;
	for (auto& tile : data) {
		pool.push_back(&tile);
	}

	Tile* topLeft = nullptr;
	for (int i = 0; i < data.size(); ++i) {
		auto borderType = lookForBorder(pool, i);
		if (borderType == TOP_LEFT) {
			topLeft = pool[i];
			pool.erase(pool.begin() + i);
			break;
		}
	}
	grid[0][0] = topLeft;

	// fill top border
	auto previous = topLeft;
	for (int i = 1; i < tilesInRow; ++i) {
		auto index = findMatchingLeft(previous->right, pool);
		if (index < 0) {
			std::cout << "No tile found" << std::endl;
			return;
		}
		previous = pool[index];
		grid[0][i] = previous;
		pool.erase(pool.begin() + index);
	}

	for (int x = 0; x < tilesInRow; ++x) {
		previous = grid[0][x];
		for (int i = 1; i < tilesInRow; ++i) {
			auto index = findMatchingTop(previous->bottom, pool);
			if (index < 0) {
				std::cout << "No tile found" << std::endl;
				return;
			}
			previous = pool[index];
			grid[i][x] = previous;
			pool.erase(pool.begin() + index);
		}
	}

	/*for (const auto& gridRow : grid) {
		const auto tileSize = gridRow.front()->map.size();
		int tileIndex = 0;
		for (int i = 0; i < tileSize; ++i) {
			for (int j = 0; j < tileSize * gridRow.size(); ++j) {
				std::cout << gridRow[tileIndex]->map[i][j % tileSize];
				tileIndex = j / tileSize;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/

	// trim
	for (auto& row : grid) {
		for (auto& tile : row) {
			tile->map.pop_front();
			tile->map.pop_back();
			for (auto& mapRow : tile->map) {
				mapRow.pop_back();
				mapRow.pop_front();
			}
		}
	}

	const int newTileSize = topLeft->map.size();
	const int seaSize = tilesInRow * newTileSize;

	std::vector<std::vector<char>> seaMap(seaSize, std::vector<char>(seaSize, '\0'));
	for (int y = 0; y < grid.size(); ++y) {
		for (int x = 0; x < grid.size(); ++x) {
			const auto& map = grid[y][x]->map;
			for (int i = 0; i < map.size(); ++i) {
				for (int j = 0; j < map[i].size(); ++j) {
					seaMap[y * newTileSize + i][x * newTileSize + j] = map[i][j];
				}
			}
		}
	}

	int monsters = 0;
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	rotateMatrix(seaMap);
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	rotateMatrix(seaMap);
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	rotateMatrix(seaMap);
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	std::reverse(seaMap.begin(), seaMap.end());
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	rotateMatrix(seaMap);
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	rotateMatrix(seaMap);
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	rotateMatrix(seaMap);
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}

	std::reverse(seaMap.begin(), seaMap.end());
	rotateMatrix(seaMap);
	rotateMatrix(seaMap);
	rotateMatrix(seaMap);
	for (const auto& row : seaMap) {
		for (const auto& value : row) {
			std::cout << value;
		}
		std::cout << std::endl;
	}

	int hashCount = 0;
	for (const auto& row : seaMap) {
		for (const auto& value : row) {
			if (value == '#') {
				++hashCount;
			}
		}
	}

	std::cout << "Part two: " << hashCount << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}