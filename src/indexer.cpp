#include <bits/stdc++.h>
#include <fstream>
#include <filesystem>
#include <indexer.hpp>

namespace fs = std::filesystem;

using namespace std;
using fs::directory_iterator;

void indexDirectory(const fs::path& dirPath, InvertedIndex& index) {
	for (const auto& entry: directory_iterator(dirPath)) {
		if (entry.is_directory()) {
			indexDirectory(entry.path(), index);
		}
		else {
			index.indexFile(entry.path());
		}
	}
}

int main(void) {
	InvertedIndex index;
	fs::path dirpath = "./sample_files";
	fs::path fileName = "index.json";

	indexDirectory(dirpath, index);
	
	fs::path fullPath = dirpath / fileName;
	index.saveIndexToJson(fullPath.string());
}