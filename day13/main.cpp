#include "../common/fileReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

struct Departure {
	int busId;
	bool isX;
};

struct BusData {
	int timestamp;
	std::vector<Departure> departures;
};

using DataType = BusData;

DataType read() {
	common::FileReader reader("input.txt");
	DataType data;
	const auto timestamp = reader.nextLine();
	data.timestamp = std::stoi(timestamp);
	auto busesStr = reader.nextLine();
	const std::string delimiter = ",";
	size_t pos = 0;
	
	while ((pos = busesStr.find(delimiter)) != std::string::npos) {
		std::string token = busesStr.substr(0, pos);
		Departure departure;
		if (token != "x") {
			departure.busId = std::stoi(token);
			departure.isX = false;
		} else {
			departure.isX = true;
		}
		data.departures.push_back(departure);
		busesStr.erase(0, pos + delimiter.length());
	}
	if (busesStr != "x") {
		data.departures.push_back(Departure{ std::stoi(busesStr), false });
	} else {
		data.departures.push_back(Departure{ 0, true });
	}
	return data;
}

void partOne(const DataType& data) {
	const auto myTimestamp = data.timestamp;
	int minTimeToWait = 100;
	int minBusId = 0;
	for (const auto [busId, isX] : data.departures) {
		if (isX) continue;

		int timeToWait = (myTimestamp / busId) * busId - myTimestamp;
		if (timeToWait < 0) {
			timeToWait += busId;
		}
		if (timeToWait < minTimeToWait) {
			minTimeToWait = timeToWait;
			minBusId = busId;
		}
	}
	std::cout << "Part one: " << minBusId * minTimeToWait << std::endl;
}

std::vector<std::pair<int, int>> getOffsetFromStart(const std::vector<Departure>& departures) {
	int offset = 0;
	std::vector<std::pair<int, int>> array;
	for (const auto [busId, isX] : departures) {
		if (!isX) {
			array.emplace_back(busId, offset);
		}
		++offset;
	}
	return array;
}

void partTwo(const DataType& data) {
	// pairs: busId - offset
	std::vector<std::pair<int, int>> offsets = getOffsetFromStart(data.departures);
	std::sort(offsets.begin(), offsets.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.first > rhs.first;
	});

	// calculate initial timestamps for biggest bus id
	const auto [maxBusId, maxBusOffset] = offsets.front();
	uint64_t timestampStep = maxBusId;
	uint64_t timestamp = maxBusId - maxBusOffset;

	// find timestamp step where all numbers align
	for (int i = 1; i < offsets.size()-1; ++i) {
		const auto [secondBusId, secondOffset] = offsets[i];
		while ((timestamp + secondOffset) % secondBusId != 0) {
			timestamp += timestampStep;
		}
		const auto firstTimeStamp = timestamp;
		timestamp += timestampStep;
		while ((timestamp + secondOffset) % secondBusId != 0) {
			timestamp += timestampStep;
		}
		timestampStep = timestamp - firstTimeStamp;
		timestamp = firstTimeStamp;
	}

	// basing on calculated step, find valid timestamp
	while (true) {
		bool found = true;;
		for (const auto [busId, offset] : offsets) {
			if ((timestamp + offset) % busId != 0) {
				found = false;
				break;
			}
		}
		if (!found) {
			timestamp += timestampStep;
		} else {
			break;
		}
	}
	std::cout << "Part two: " << timestamp << std::endl;
}

int main() {
	const auto data = read();
	partOne(data);
	partTwo(data);
	return 0;
}