#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <deque>

enum CubeState {
	INACTIVE,
	ACTIVE
};

using DataType = std::deque<std::deque<std::deque<CubeState>>>;
using Data4DType = std::deque<DataType>;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data(1);
	for (std::string line; reader.nextLine(line);) {
		std::deque<CubeState> row;
		for (auto c : line) {
			if (c == '#') {
				row.push_back(ACTIVE);
			}
			else {
				row.push_back(INACTIVE);
			}
		}
		data[0].push_back(row);
	}
	return data;
}

int countCubes3D(const DataType& data, CubeState cubeState) {
	int count = 0;
	for (const auto& plane : data) {
		for (const auto& row : plane){
			for (const auto value : row) {
				if (value == cubeState) {
					++count;
				}
			}
		}
	}
	return count;
}

int countActiveNeighbours3D(const DataType& data, const int z, const int y, const int x) {
	const int zMax = data.size();
	const int yMax = data.front().size();
	const int xMax = data.front().front().size();

	int active = 0;
	for (int currentZ = z - 1; currentZ <= z + 1; ++currentZ) {
		for (int currentY = y - 1; currentY <= y + 1; ++currentY) {
			for (int currentX = x - 1; currentX <= x + 1; ++currentX) {
				if (currentX >= 0 && currentX < xMax &&
					currentY >= 0 && currentY < yMax &&
					currentZ >= 0 && currentZ < zMax &&
					(currentZ != z || currentY != y || currentX != x)) {
					active += (data[currentZ][currentY][currentX] == ACTIVE);
				}
			}
		}
	}
	return active;
}

void enlargePlane3D(DataType& data) {
	std::deque<CubeState> emptyRow(data[0][0].size(), INACTIVE);
	std::deque<std::deque<CubeState>> emptyPlane(data[0].size(), emptyRow);
	data.push_front(emptyPlane);
	data.push_back(emptyPlane);
	for (auto& plane : data) {
		plane.push_front(emptyRow);
		plane.push_back(emptyRow);
		for (auto& row : plane) {
			row.push_back(INACTIVE);
			row.push_front(INACTIVE);
		}
	}
}

int countCubes4D(const Data4DType& data, CubeState cubeState) {
	int count = 0;
	for (const auto& dimension : data) {
		for (const auto& plane : dimension) {
			for (const auto& row : plane) {
				for (const auto value : row) {
					if (value == cubeState) {
						++count;
					}
				}
			}
		}
	}
	return count;
}

int countActiveNeighbours4D(const Data4DType& data, const int w, const int z, const int y, const int x) {
	const int wMax = data.size();
	const int zMax = data.front().size();
	const int yMax = data.front().front().size();
	const int xMax = data.front().front().front().size();

	int active = 0;
	for (int currentW = w - 1; currentW <= w + 1; ++currentW) {
		for (int currentZ = z - 1; currentZ <= z + 1; ++currentZ) {
			for (int currentY = y - 1; currentY <= y + 1; ++currentY) {
				for (int currentX = x - 1; currentX <= x + 1; ++currentX) {
					if (currentX >= 0 && currentX < xMax &&
						currentY >= 0 && currentY < yMax &&
						currentZ >= 0 && currentZ < zMax &&
						currentW >= 0 && currentW < wMax &&
						(currentZ != z || currentY != y || currentX != x || currentW != w)) {
						active += (data[currentW][currentZ][currentY][currentX] == ACTIVE);
					}
				}
			}
		}
	}
	return active;
}

void enlargePlane4D(Data4DType& data) {
	std::deque<CubeState> emptyRow(data[0][0][0].size(), INACTIVE);
	std::deque<std::deque<CubeState>> emptyPlane(data[0][0].size(), emptyRow);
	DataType emptyDimension(data[0].size(), emptyPlane);
	data.push_front(emptyDimension);
	data.push_back(emptyDimension);
	for (auto& dimension : data) {
		dimension.push_back(emptyPlane);
		dimension.push_front(emptyPlane);
		for (auto& plane : dimension) {
			plane.push_front(emptyRow);
			plane.push_back(emptyRow);
			for (auto& row : plane) {
				row.push_back(INACTIVE);
				row.push_front(INACTIVE);
			}
		}
	}
	
}

void partOne(DataType data) {
	for (int cycle = 0; cycle < 6; ++cycle) {
		enlargePlane3D(data);
		auto newData = data;
		for (int z = 0; z < data.size(); ++z) {
			for (int y = 0; y < data[z].size(); ++y) {
				for (int x = 0; x < data[z][y].size(); ++x) {
					int activeNeighbours = countActiveNeighbours3D(data, z, y, x);
					if (data[z][y][x] == ACTIVE && activeNeighbours != 2 && activeNeighbours != 3) {
						newData[z][y][x] = INACTIVE;
					}
					else if (data[z][y][x] == INACTIVE && activeNeighbours == 3) {
						newData[z][y][x] = ACTIVE;
					}
				}
			}
		}
		data = newData;
	}
	int activeCubes = countCubes3D(data, ACTIVE);
	std::cout << "Part one: " << activeCubes << std::endl;
}

void partTwo(DataType data3D) {
	Data4DType data{ data3D };

	for (int cycle = 0; cycle < 6; ++cycle) {
		enlargePlane4D(data);
		auto newData = data;
		for (int w = 0; w < data.size(); ++w) {
			for (int z = 0; z < data[w].size(); ++z) {
				for (int y = 0; y < data[w][z].size(); ++y) {
					for (int x = 0; x < data[w][z][y].size(); ++x) {
						int activeNeighbours = countActiveNeighbours4D(data, w, z, y, x);
						if (data[w][z][y][x] == ACTIVE && activeNeighbours != 2 && activeNeighbours != 3) {
							newData[w][z][y][x] = INACTIVE;
						}
						else if (data[w][z][y][x] == INACTIVE && activeNeighbours == 3) {
							newData[w][z][y][x] = ACTIVE;
						}
					}
				}
			}
		}
		data = newData;
	}
	int activeCubes = countCubes4D(data, ACTIVE);
	std::cout << "Part two: " << activeCubes  << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}