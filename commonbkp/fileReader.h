#pragma once

#include <string>
#include <fstream>

namespace common {

class FileReader {
public:
	FileReader(const std::string& fileName);
	std::string nextLine();
	bool nextLine(std::string& line);
	std::string nextWord();
	bool nextWord(std::string& word);

private:
	const std::string mFileName;
	std::ifstream mStream;
};

}