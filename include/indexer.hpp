#pragma once

#include <vector>
#include <map>
#include <string>

using std::string;
using std::vector;
using std::map;

struct FileHit {
	string filePath;
	std::vector<size_t> lineNumbers;
};

using InvertedIndex = map<string, vector<FileHit>>;

void IndexDirectory(const string& path, InvertedIndex& index);