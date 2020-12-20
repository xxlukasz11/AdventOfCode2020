#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>

struct Tile {
	int id;
	std::string top;
	std::string left;
	std::string right;
	std::string bottom;
	std::deque<std::deque<char>> map;
};

using DataType = std::vector<Tile>;
using TileGridType = std::vector<std::vector<Tile*>>;
using SeaMapType = std::vector<std::vector<char>>;

enum BorderType {
	CENTER,
	FRAME,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

DataType read() {
	common::FileReader reader("input.txt");
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

template<typename T>
void rotateMatrix90(T&& matrix) {
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

void flipVertical(Tile& tile) {
	std::reverse(tile.left.begin(), tile.left.end());
	std::reverse(tile.right.begin(), tile.right.end());
	std::swap(tile.top, tile.bottom);
	std::reverse(tile.map.begin(), tile.map.end());
}

void flipHorizontal(Tile& tile) {
	std::reverse(tile.top.begin(), tile.top.end());
	std::reverse(tile.bottom.begin(), tile.bottom.end());
	std::swap(tile.left, tile.right);
	for (auto& row : tile.map) {
		std::reverse(row.begin(), row.end());
	}
}

void rotate180(Tile& tile) {
	flipVertical(tile);
	flipHorizontal(tile);
}

void rotate90(Tile& tile) {
	std::swap(tile.top, tile.left);
	std::swap(tile.bottom, tile.top);
	std::swap(tile.right, tile.top);
	std::reverse(tile.left.begin(), tile.left.end());
	std::reverse(tile.right.begin(), tile.right.end());
	rotateMatrix90(tile.map);
}

BorderType determineTilePlacement(std::vector<Tile*> pool, int i) {
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
	if (bottomFree || topFree || leftFree || rightFree) {
		return FRAME;
	}
	return CENTER;
}

int findMatchingLeft(const std::string& border, std::vector<Tile*>& tiles) {
	for (int i = 0; i < tiles.size(); ++i) {
		auto tile = tiles[i];
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

int findMatchingTop(const std::string& border, std::vector<Tile*>& tiles) {
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

int findMatchingTopLeft(const std::string& borderTop, const std::string& borderLeft, std::vector<Tile*>& tiles) {
	for (int i = 0; i < tiles.size(); ++i) {
		auto tile = tiles[i];
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
		flipHorizontal(*tile);
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
		rotate90(*tile);
		if (borderTop == tile->top && borderLeft == tile->left) {
			return i;
		}
	}
	return -1;
}

bool findSeaMonster(SeaMapType& seaMap, int x, int y) {
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

TileGridType assembleTiles(DataType& data) {
	const int tilesInRow = sqrt(data.size());
	TileGridType grid(tilesInRow, std::vector<Tile*>(tilesInRow, nullptr));
	std::vector<Tile*> pool;
	for (auto& tile : data) {
		pool.push_back(&tile);
	}

	Tile* topLeft = nullptr;
	std::vector<Tile*> centers;
	std::vector<Tile*> frame;
	for (int i = 0; i < pool.size(); ++i) {
		auto borderType = determineTilePlacement(pool, i);
		if (borderType == CENTER) {
			centers.push_back(pool[i]);
		} else if (borderType == TOP_LEFT) {
			topLeft = pool[i];
		} else {
			frame.push_back(pool[i]);
		}
	}
	grid[0][0] = topLeft;


	// fill top border
	auto previous = topLeft;
	for (int i = 1; i < tilesInRow; ++i) {
		auto index = findMatchingLeft(previous->right, frame);
		previous = frame[index];
		grid[0][i] = previous;
		frame.erase(frame.begin() + index);
	}

	// fill left border
	previous = topLeft;
	for (int i = 1; i < tilesInRow; ++i) {
		auto index = findMatchingTop(previous->bottom, frame);
		previous = frame[index];
		grid[i][0] = previous;
		frame.erase(frame.begin() + index);
	}

	// fill bottom border
	previous = grid[tilesInRow - 1][0];
	for (int i = 1; i < tilesInRow; ++i) {
		auto index = findMatchingLeft(previous->right, frame);
		previous = frame[index];
		grid[tilesInRow - 1][i] = previous;
		frame.erase(frame.begin() + index);
	}

	// fill right border
	previous = grid[0][tilesInRow - 1];
	for (int i = 1; i < tilesInRow - 1; ++i) {
		auto index = findMatchingTop(previous->bottom, frame);
		previous = frame[index];
		grid[i][tilesInRow - 1] = previous;
		frame.erase(frame.begin() + index);
	}

	// fill center
	Tile* previousTop;
	Tile* previousLeft;
	for (int x = 1; x < tilesInRow - 1; ++x) {
		previousTop = grid[0][x];
		for (int i = 1; i < tilesInRow - 1; ++i) {
			previousLeft = grid[i][x - 1];
			auto index = findMatchingTopLeft(previousTop->bottom, previousLeft->right, centers);
			previousTop = centers[index];
			grid[i][x] = previousTop;
			centers.erase(centers.begin() + index);
		}
	}
	return grid;
}

void printAssembledTiles(const TileGridType& grid) {
	for (const auto& gridRow : grid) {
		const auto tilesInRow = gridRow.size();
		const auto tileSize = gridRow.front()->map.size();
		const auto totalSteps = tilesInRow * tileSize;
		for (int mapRow = 0; mapRow < tileSize; ++mapRow) {
			bool spaced = true;
			for (int i = 0; i < totalSteps; ++i) {
				if (i % tileSize == 0 && !spaced) {
					spaced = true;
					--i;
					std::cout << " ";
				} else {
					spaced = false;
					const auto tileIndex = i / tileSize;
					const auto mapColumn = i % tileSize;
					std::cout << gridRow[tileIndex]->map[mapRow][mapColumn];
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

SeaMapType generateSeaMap(TileGridType& grid) {
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

	const int tilesInRow = grid.size();
	const int newTileSize = grid[0][0]->map.size();
	const int seaSize = tilesInRow * newTileSize;

	SeaMapType seaMap(seaSize, std::vector<char>(seaSize, '\0'));
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
	return seaMap;
}

int countSeaMonsters(SeaMapType& seaMap) {
	const int seaSize = seaMap.size();
	int monsters = 0;
	for (int y = 0; y < seaSize; ++y) {
		for (int x = 0; x < seaSize; ++x) {
			if (findSeaMonster(seaMap, x, y)) {
				++monsters;
			}
		}
	}
	return monsters;
}

void printSeaMap(const SeaMapType& seaMap) {
	for (const auto& row : seaMap) {
		for (const auto& value : row) {
			std::cout << value;
		}
		std::cout << std::endl;
	}
}

void partOne(DataType data) {
	std::vector<Tile*> pool;
	for (auto& tile : data) {
		pool.push_back(&tile);
	}

	int64_t mul = 1;
	for (int i = 0; i < data.size(); ++i) {
		auto placement = determineTilePlacement(pool, i);
		if (placement == TOP_LEFT || placement == TOP_RIGHT || placement == BOTTOM_LEFT || placement == BOTTOM_RIGHT) {
			mul *= data[i].id;
		}
	}
	std::cout << "Part one: " << mul << std::endl;
}

void partTwo(DataType data) {
	TileGridType grid = assembleTiles(data);
	printAssembledTiles(grid);
	SeaMapType seaMap = generateSeaMap(grid);

	int monsters = countSeaMonsters(seaMap);
	rotateMatrix90(seaMap);
	monsters += countSeaMonsters(seaMap);
	rotateMatrix90(seaMap);
	monsters += countSeaMonsters(seaMap);
	rotateMatrix90(seaMap);
	monsters += countSeaMonsters(seaMap);
	std::reverse(seaMap.begin(), seaMap.end());
	monsters += countSeaMonsters(seaMap);
	rotateMatrix90(seaMap);
	monsters += countSeaMonsters(seaMap);
	rotateMatrix90(seaMap);
	monsters += countSeaMonsters(seaMap);
	rotateMatrix90(seaMap);
	monsters += countSeaMonsters(seaMap);

	printSeaMap(seaMap);

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