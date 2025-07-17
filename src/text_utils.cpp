#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <text_utils.hpp>
#include <stemming/english_stem.h>

using std::string;
using std::vector;
using std::regex_replace;
using std::regex;
using std::cout;
using std::stringstream;
using stemming::english_stem;
using std::wstring;

string textToLowerCase(const string& text) {
	string lowercased = "";
	for (char c: text) lowercased.push_back(tolower(c));
	return lowercased;
}

string stripPunctuation(const string& text) {
	string punctuationsRemovedText = "";
	string textCpy = text;
	regex r("[^a-zA-Z0-9 \n]*");

	regex_replace(back_inserter(punctuationsRemovedText), textCpy.begin(), textCpy.end(), r, "");

	return punctuationsRemovedText;
}

string stem(const string& word) {
	english_stem<wstring> stemmer;
	wstring wordCpy(word.begin(), word.end());

	stemmer(wordCpy);

	return string(wordCpy.begin(), wordCpy.end());
}

vector<string> tokenize(const string& text) {
	vector<string> tokens;
	stringstream words(text);

	string word;
	while (words >> word) {
		tokens.push_back(word);
	}

	return tokens;
}

vector<string> sentTokenize(const string& text) {
	vector<string> sentTokens;

	string sentence = "";
	for (char c: text) {
		if (c == '\n') {
			sentTokens.push_back(sentence);
			sentence = "";
		}
		else sentence.push_back(c);
	}

	if (sentence.length() > 0) {
		sentTokens.push_back(sentence);
	}

	return sentTokens;
}

string joinTokens(const vector<string>& tokens) {
	string joinedTokenString = "";

	for (string token: tokens) {
		joinedTokenString.append(token);
		joinedTokenString.push_back(' ');
	}

	return joinedTokenString;
}

string cleanText(const string& text) {
	string lowercased = textToLowerCase(text);
	string textPunctationsRemoved = stripPunctuation(lowercased);


	vector<string> tokens = tokenize(textPunctationsRemoved);
	int n = tokens.size();
	for (int i = 0; i < n; i++) {
		tokens[i] = stem(tokens[i]);
	}

	string joinedTokenString = joinTokens(tokens);
	return joinedTokenString;
}
