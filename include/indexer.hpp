#pragma once

#include <vector>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include <json.hpp>
#include <text_utils.hpp>

namespace fs = std::filesystem;

using std::map;
using std::string;
using std::vector;
using std::set;
using json = nlohmann::json;
using std::stringstream;
using std::ifstream;

struct FileHit {
	string filePath;
	std::vector<size_t> lineNumbers;
};

void to_json(json& j, const FileHit& hit) {
	j = json{
		{ "filePath", hit.filePath },
		{ "lineNumbers", hit.lineNumbers }
	};
}

class InvertedIndex {
private:
	set<string> parsedFilePaths;
	size_t corpusSize = 0;

public:
	map<string, vector<FileHit>> index;

	bool isFileParsed(const string& filePath) {
		bool fileNotParsed = (this->parsedFilePaths).find(filePath) == (this->parsedFilePaths).end();
		return !fileNotParsed;
	}

	void saveIndexToJson(const string& filePath) {
		json j;

		for (const auto& [word, hits]: this->index) {
			j[word] = hits;
		}

		std::ofstream file(filePath);

		// indents 4 spaces per json operation
		file << std::setw(4) << j << std::endl;
		file.close();
	}

	void updateIndex() {

	}

	void indexFile(const fs::path& filePath) {
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
					for (auto& hit: (this->index)[word]) {
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
						(this->index)[word].push_back(hit);
					}
		
				}
			}

			i++;
		}
	}

};

void IndexDirectory(const string &path, InvertedIndex &index);