#include <bits/stdc++.h>
#include <fstream>
#include <filesystem>
#include <indexer.hpp>
#include <text_utils.hpp>
#include <utils.hpp>

namespace fs = std::filesystem;

using namespace std;
using fs::directory_iterator;

void IndexFile(const fs::path& filePath, InvertedIndex& index) {
	ifstream file(filePath);
	string filePathStr = filePath.string();

	stringstream buffer;
	buffer << file.rdbuf();

	vector<string> sentences = sentTokenize(buffer.str());

	int i = 0;
	for (const string& sentence: sentences) {
		string content = cleanText(sentence);
		vector<string> words = tokenize(content);

		set<string> wordEncounteredInSentence;

		for (const string& word: words) {
			if (wordEncounteredInSentence.find(word) == wordEncounteredInSentence.end()) {
				wordEncounteredInSentence.insert(word);
				
				bool found = false;
				for (auto& hit: index[word]) {
					if (hit.filePath == filePathStr) {
						hit.lineNumbers.push_back(i+1);
						found = true;
						break;
					}
				}

				if (!found) {
					FileHit hit;
					hit.filePath = filePathStr;
					hit.lineNumbers.push_back(i+1);
					index[word].push_back(hit);
				}
	
			}
		}

		i++;
	}
}

void IndexDirectory(const fs::path& dirPath, InvertedIndex& index) {
	for (const auto& entry: directory_iterator(dirPath)) {
		if (entry.is_directory()) {
			IndexDirectory(entry.path(), index);
		}
		else {
			IndexFile(entry.path(), index);
		}
	}
}

int main(void) {
	InvertedIndex index;
	fs::path dirpath = "./sample_files";
	fs::path fileName = "index.json";

	IndexDirectory(dirpath, index);
	
	fs::path fullPath = dirpath / fileName;
	saveIndexToJson(index, fullPath.string());
}