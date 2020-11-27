#include "pch.h"
#include "fileReader.h"

namespace common {

FileReader::FileReader(const std::string& fileName) : mFileName(fileName) {
	mStream.open(mFileName);
	if (!mStream) {
		throw std::runtime_error("Cannot find file: " + mFileName);
	}
}

std::string FileReader::nextLine() {
	std::string line;
	std::getline(mStream, line);
	return line;
}

bool FileReader::nextLine(std::string& line) {
	return static_cast<bool>(std::getline(mStream, line));
}

std::string FileReader::nextWord() {
	std::string word;
	mStream >> word;
	return word;
}

bool FileReader::nextWord(std::string& word) {
	return static_cast<bool>(mStream >> word);
}

}