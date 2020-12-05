#include <iostream>
#include "../common/fileReader.h"
#include <vector>
#include <string>

struct Seat {
	int row;
	int col;
};

constexpr char FRONT = 'F';
constexpr char BACK = 'B';
constexpr char LEFT = 'L';
constexpr char RIGHT = 'R';

Seat decode(const std::string& str) {
	Seat seat{ 0, 0 };
	for (const auto symbol : str) {
		switch (symbol) {
			case BACK: {
				seat.row <<= 1;
				seat.row |= 1;
			} break;
			case FRONT: {
				seat.row <<= 1;
			} break;
			case RIGHT: {
				seat.col <<= 1;
				seat.col |= 1;
			} break;
			case LEFT: {
				seat.col <<= 1;
			} break;
		}
	}
	return seat;
}

std::vector<Seat> decodeSeats(const std::vector<std::string>& seats) {
	std::vector<Seat> decoded;
	decoded.reserve(seats.size());
	for (const auto& seat : seats) {
		decoded.push_back(decode(seat));
	}
	return decoded;
}

std::vector<Seat> generateMissingSeats(const int rows, const int cols, const std::vector<Seat>& existingSeats) {
	std::vector<Seat> missing;
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			Seat seat{ row, col };
			if (std::find_if(existingSeats.begin(), existingSeats.end(), [&seat](const auto& f) {
				return f.row == seat.row && f.col == seat.col;
				}) == existingSeats.cend()) {
				missing.push_back(seat);
			}
		}
	}
	return missing;
}

int calculateId(const Seat& seat) {
	return seat.row * 8 + seat.col;
}

void partOne(const std::vector<Seat>& seats) {
	int highestId = 0;
	for (const auto seat : seats) {
		const auto id = calculateId(seat);
		if (id > highestId) {
			highestId = id;
		}
	}
	std::cout << "Part one - Highest ID: " << highestId << std::endl;
}

void partTwo(const std::vector<Seat>& seats) {
	std::vector<Seat> missing = generateMissingSeats(128, 8, seats);
	int previousId = calculateId(missing[0]) - 1;
	for (const auto& seat : missing) {
		int currentId = calculateId(seat);
		if (currentId == previousId + 1) {
			previousId = currentId;
		} else {
			std::cout << "Part two - R: " << seat.row << " - C: " << seat.col << "\n";
			break;
		}
	}
}

int main() {
	common::FileReader reader("input.txt");
	std::vector<std::string> seats;
	for (std::string line; reader.nextLine(line);) {
		seats.push_back(line);
	}
	std::vector<Seat> decodedSeats = decodeSeats(seats);
	partOne(decodedSeats);
	partTwo(decodedSeats);
	return 0;
}