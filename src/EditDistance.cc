#include "EditDistance.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

EditDistance::EditDistance()
{
}

EditDistance::EditDistance(string baseString, DistanceAlgorithm algorithm)
{
	this->baseString = baseString;
	this->algorithm = algorithm;
	if (this->baseString.empty()) {
		this->baseString = "";
		return;
	}
	DistanceAlgorithm algo= Damerau;
	if (algorithm == algo) {
		v0 = vector<int>(baseString.size());
		v2 = vector<int>(baseString.size()); // stores one level further back (offset by +1 position)
	}
}

EditDistance::~EditDistance()
{
}

int EditDistance::compare(string string2, int maxDistance) {
	switch (algorithm) {
	case Damerau:
		return DamerauLevenshteinDistance(string2, maxDistance);
	default:
		return DamerauLevenshteinDistance(string2, maxDistance);
	}
}

int EditDistance::DamerauLevenshteinDistance(string string2, int maxDistance) {
	if (baseString == "") return string2 == "" ? 0 : string2.size(); //string2 ?? "").Length;
	if (string2 == "" || string2.empty()) return baseString.size();

	// if strings of different lengths, ensure shorter string is in string1. This can result in a little
	// faster speed by spending more time spinning just the inner loop during the main processing.
	string string1;
	if (baseString.size() > string2.size()) {
		string1 = string2;
		string2 = baseString;
	}
	else {
		string1 = baseString;
	}
	int sLen = string1.size(); // this is also the minimun length of the two strings
	int tLen = string2.size();

	// suffix common to both strings can be ignored
	while ((sLen > 0) && (string1.at(sLen - 1) == string2.at(tLen - 1))) { sLen--; tLen--; }

	int start = 0;
	if ((string1.at(0) == string2.at(0)) || (sLen == 0)) { // if there'string1 a shared prefix, or all string1 matches string2'string1 suffix
																   // prefix common to both strings can be ignored
		while ((start < sLen) && (string1.at(start) == string2.at(start))) start++;
		sLen -= start; // length of the part excluding common prefix and suffix
		tLen -= start;

		// if all of shorter string matches prefix and/or suffix of longer string, then
		// edit distance is just the delete of additional characters present in longer string
		if (sLen == 0) return tLen;

		string2 = string2.substr(start, start + tLen); // faster than string2[start+j] in inner loop below
	}
	int lenDiff = tLen - sLen;
	if ((maxDistance < 0) || (maxDistance > tLen)) {
		maxDistance = tLen;
	}
	else if (lenDiff > maxDistance) return -1;

	if (tLen > v0.size())
	{
		v0 = vector<int>(tLen);
		v2 = vector<int>(tLen);
	}
	else {
		for (int i = 0; i < tLen; i++) v2[i] = 0;    // Substituting Array.clear(v2, 0, tLen)
	}
	int j;
	for (j = 0; j < maxDistance; j++) v0[j] = j + 1;
	for (; j < tLen; j++) v0[j] = maxDistance + 1;

	int jStartOffset = maxDistance - (tLen - sLen);
	bool haveMax = maxDistance < tLen;
	int jStart = 0;
	int jEnd = maxDistance;
	char sChar = string1.at(0);
	int current = 0;
	for (int i = 0; i < sLen; i++) {
		char prevsChar = sChar;
		sChar = string1.at(start + i);
		char tChar = string2.at(0);
		int left = i;
		current = left + 1;
		int nextTransCost = 0;
		// no need to look beyond window of lower right diagonal - maxDistance cells (lower right diag is i - lenDiff)
		// and the upper left diagonal + maxDistance cells (upper left is i)
		jStart += (i > jStartOffset) ? 1 : 0;
		jEnd += (jEnd < tLen) ? 1 : 0;
		for (j = jStart; j < jEnd; j++) {
			int above = current;
			int thisTransCost = nextTransCost;
			nextTransCost = v2[j];
			v2[j] = current = left; // cost of diagonal (substitution)
			left = v0[j];    // left now equals current cost (which will be diagonal at next iteration)
			char prevtChar = tChar;
			tChar = string2.at(j);
			if (sChar != tChar) {
				if (left < current) current = left;   // insertion
				if (above < current) current = above; // deletion
				current++;
				if ((i != 0) && (j != 0)
					&& (sChar == prevtChar)
					&& (prevsChar == tChar)) {
					thisTransCost++;
					if (thisTransCost < current) current = thisTransCost; // transposition
				}
			}
			v0[j] = current;
		}
		if (haveMax && (v0[i + lenDiff] > maxDistance)) return -1;
	}
	return (current <= maxDistance) ? current : -1;
}


int EditDistance::EditDistanceRe(string left, string right)
{
	int l_length = left.length();
	int r_length = right.length();

	if (left.length() == 0) return r_length;
	else if (right.length() == 0) return l_length;
	else if (left.substr(l_length - 1) == right.substr(r_length - 1)) {
		//cout << "Last char maches" << endl;
		return EditDistanceRe(left.substr(0, l_length - 1), right.substr(0, r_length - 1));
	}
	else{
		int a = EditDistanceRe(left.substr(0, l_length - 1), right) + 1;
		int b = EditDistanceRe(left, right.substr(0, r_length - 1)) + 1;
		int c = EditDistanceRe(left.substr(0, l_length - 1), right.substr(0, r_length - 1)) +1;
		return min(a, min(b, c));
	}

}

int EditDistance::EditDistanceDp(string left, string right)
{
	int l_length = left.length();;
	int r_length = right.length();
	vector< vector<int>> cache(l_length+1, vector<int>(r_length+1));
	int i, j;
	for (i = 0; i <= l_length; i++) {
		cache[i][0] = i;
	}
	for (j = 0; j <= r_length; j++) {
		cache[0][j] = j;
	}

	for (i = 1; i <= l_length; i++) {
		for (j = 1; j <= r_length; j++) {
			if (left.at(i-1) == right.at(j-1)) {
				cache[i][j] = cache[i - 1][j - 1];
			}
			else {
				int a = cache[i - 1][j] + 1;
				int b = cache[i][j - 1] + 1;
				int c = cache[i - 1][j - 1] + 1;
				cache[i][j] = min(a, min(b, c));
			}
		}
	}
	return cache[l_length][r_length];
}

int EditDistance::EidtDistanceOp(string left, string right)
{
	int l_length = left.length();
	int r_length = right.length();
	vector<int> head(r_length + 1, 0);
	vector<int> prev(r_length + 1, 0);
	for (int i = 0; i < r_length+1; i++) {
		prev[i] = i;
	}
	for (int k = 1; k < l_length+1; k++) {
		head[0] = k;
		for (int m = 1; m < r_length + 1; m++) {
			if (left.at(k-1) == right.at(m-1)) {
				head[m] = prev[m - 1];
				cout << head[m] << endl;
			}
			else {
				int a = head[m - 1] + 1;
				int b = prev[m] + 1;
				int c = prev[m - 1] + 1;
				head[m] = min(a, min(b, c));
			}
		}
		prev = head;
	}
	return head[r_length];
}
