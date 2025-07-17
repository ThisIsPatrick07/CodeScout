#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

string textToLowerCase(const string& text);
string stripPunctuation(const string& text);
string stem(const string& text);
vector<string> tokenize(const string& text);
vector<string> sentTokenize(const string& text);
string joinTokens(const vector<string>& tokens);
string cleanText(const string& text);