#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <memory>
#include "../common/fileReader.h"
#include "document.h"

std::vector<Document> readDocuments() {
	common::FileReader reader("input.txt");
	std::vector<Document> documents;
	for (std::string docStr; reader.nextLine(docStr);) {
		std::string nextLine;
		while (reader.nextLine(nextLine) && nextLine != "") {
			docStr += " " + nextLine;
		}
		documents.push_back(Document::parse(docStr));
	}
	return documents;
}

void partOne(const std::vector<Document>& documents) {
	int validDocuments = 0;
	for (const auto& doc : documents) {
		if (doc.hasAllRequiredFields()) {
			++validDocuments;
		}
	}
	std::cout << "Part one - Valid documents: " << validDocuments << std::endl;
}

void partTwo(const std::vector<Document>& documents) {
	int validDocuments = 0;
	for (const auto& doc : documents) {
		if (doc.hasValidFields()) {
			++validDocuments;
		}
	}
	std::cout << "Part two - Valid documents: " << validDocuments << std::endl;
}

int main() {
	std::vector<Document> documents = readDocuments();
	partOne(documents);
	partTwo(documents);
	return 0;
}