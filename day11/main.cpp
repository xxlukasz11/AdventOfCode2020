#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>

enum Seat {
	FLOOR,
	EMPTY,
	OCCUPIED
};

using DataType = std::vector<std::vector<Seat>>;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	for (std::string line; reader.nextLine(line);) {
		std::vector<Seat> row;
		for (char c : line) {
			if (c == '#') {
				row.push_back(OCCUPIED);
			} else if (c == 'L') {
				row.push_back(EMPTY);
			} else {
				row.push_back(FLOOR);
			}
		}
		data.push_back(row);
	}
	return data;
}

int countOccupiedSeatsInDirections(const DataType& data, int row, int col) {
	const int rows = data.size();
	const int cols = data.back().size();
	int occupied = 0;
	for (int i = row - 1; i >= 0; --i) {
		if (data[i][col] > FLOOR) {
			occupied += data[i][col] == OCCUPIED;
			break;
		}
	}
	for (int i = row + 1; i < rows; ++i) {
		if (data[i][col] > FLOOR) {
			occupied += data[i][col] == OCCUPIED;
			break;
		}
	}
	for (int i = col - 1; i >= 0; --i) {
		if (data[row][i] > FLOOR) {
			occupied += data[row][i] == OCCUPIED;
			break;
		}
	}
	for (int i = col + 1; i < cols; ++i) {
		if (data[row][i] > FLOOR) {
			occupied += data[row][i] == OCCUPIED;
			break;
		}
	}
	for (int i = row - 1, j = col -1; i >= 0 && j >= 0; --i, --j) {
		if (data[i][j] > FLOOR) {
			occupied += data[i][j] == OCCUPIED;
			break;
		}
	}
	for (int i = row - 1, j = col + 1; i >= 0 && j < cols; --i, ++j) {
		if (data[i][j] > FLOOR) {
			occupied += data[i][j] == OCCUPIED;
			break;
		}
	}
	for (int i = row + 1, j = col + 1; i < rows && j < cols; ++i, ++j) {
		if (data[i][j] > FLOOR) {
			occupied += data[i][j] == OCCUPIED;
			break;
		}
	}
	for (int i = row + 1, j = col - 1; i < rows && j >= 0; ++i, --j) {
		if (data[i][j] > FLOOR) {
			occupied += data[i][j] == OCCUPIED;
			break;
		}
	}
	return occupied;
}

int countOccupiedSeatsInNeighborhood(const DataType& data, int row, int col) {
	const int rows = data.size();
	const int cols = data.back().size();
	const auto seat = data[row][col];
	int occupied = 0;
	if (row != 0) {
		occupied += data[row - 1][col] == OCCUPIED;
		if (col != 0) {
			occupied += data[row - 1][col - 1] == OCCUPIED;
		}
		if (col != cols - 1) {
			occupied += data[row - 1][col + 1] == OCCUPIED;
		}
	}
	if (row != rows - 1) {
		occupied += data[row + 1][col] == OCCUPIED;
		if (col != 0) {
			occupied += data[row + 1][col - 1] == OCCUPIED;
		}
		if (col != cols - 1) {
			occupied += data[row + 1][col + 1] == OCCUPIED;
		}
	}
	if (col != 0) {
		occupied += data[row][col - 1] == OCCUPIED;
	}
	if (col != cols - 1) {
		occupied += data[row][col + 1] == OCCUPIED;
	}
	return occupied;
}

Seat changeSeatPartTwo(const DataType& data, int row, int col) {
	const auto seat = data[row][col];
	const auto occupied = countOccupiedSeatsInDirections(data, row, col);
	if (seat == EMPTY && occupied == 0) {
		return OCCUPIED;
	}
	if (seat == OCCUPIED && occupied >= 5) {
		return EMPTY;
	}
	return seat;
}

Seat changeSeatPartOne(const DataType& data, int row, int col) {
	const auto seat = data[row][col];
	const int occupied = countOccupiedSeatsInNeighborhood(data, row, col);
	if (seat == EMPTY && occupied == 0) {
		return OCCUPIED;
	}
	if (seat == OCCUPIED && occupied >= 4) {
		return EMPTY;
	}
	return seat;
}

auto determineChangeSeatFunction(bool isPartOne) {
	if (isPartOne) {
		return changeSeatPartOne;
	}
	return changeSeatPartTwo;
}

bool nextIteration(DataType& data, bool isPartOne) {
	const auto changeSeat = determineChangeSeatFunction(isPartOne);
	DataType newData = data;
	bool changed = false;
	const int rows = data.size();
	const int cols = data.back().size();
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			const auto seat = data[i][j];
			if (seat != FLOOR) {
				const auto newSeat = changeSeat(data, i, j);
				newData[i][j] = newSeat;
				if (seat != newSeat) {
					changed = true;
				}
			}
		}
	}
	data = newData;
	return changed;
}

int countSeats(const DataType& data, Seat seat) {
	int count = 0;
	for (const auto& row : data) {
		for (const auto s : row) {
			if (s == seat) {
				++count;
			}
		}
	}
	return count;
}

void partOne(DataType data) {
	while (nextIteration(data, true));
	const int occupied = countSeats(data, OCCUPIED);
	std::cout << "Part one: " << occupied << std::endl;
}

void partTwo(DataType data) {
	while (nextIteration(data, false));
	const int occupied = countSeats(data, OCCUPIED);
	std::cout << "Part two: " << occupied << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}