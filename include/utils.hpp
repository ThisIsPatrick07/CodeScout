#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <indexer.hpp>
#include <json.hpp>

using std::vector;
using std::cout;
using std::ostream;
using json = nlohmann::json;

template<typename T>
void printVector(const vector<T>& arr, string separator = " ") {
	for (auto ele: arr) {
		cout << ele << separator;
	}
	cout << std::endl;
}

void to_json(json& j, const FileHit& hit) {
	j = json{
		{ "filePath", hit.filePath },
		{ "lineNumbers", hit.lineNumbers }
	};
}

void saveIndexToJson(InvertedIndex& index, const string& filePath) {
	json j;

	for (const auto& [word, hits]: index) {
		j[word] = hits;
	}

	std::ofstream file(filePath);

	// indents 4 spaces per json operation
	file << std::setw(4) << j << std::endl;
	file.close();
}


/*
	This function allows for cout << hit, where hit is of type FileHit, 
	to print out the custom strings that have been coded in this function.
*/
inline ostream& operator<<(ostream& os, const FileHit& hit) {
	os << "{ file: " << hit.filePath << ", lines: [";

	int numLines = hit.lineNumbers.size();
	for (size_t i = 0; i < numLines; i++) {
		os << hit.lineNumbers[i];
		if (i < numLines-1) os << ", ";
	}

	os << "] }";
	return os;
}

inline ostream& operator<<(ostream& os, const InvertedIndex& index) {
	for (auto wordHitPair: index) {
		string word = wordHitPair.first;
		vector<FileHit> hits = wordHitPair.second;

		os << word << ": \n\t";

		printVector(hits, "\n\t");
	}

	os << "\n\n";
	return os;
}

