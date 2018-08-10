#pragma once
#include <string>

using namespace std;

class SuggestItem
{
public:
	SuggestItem();
	SuggestItem(string term, int distance, unsigned long long int count);
	~SuggestItem();
	int CompareTo(SuggestItem other);
	bool operator==(SuggestItem other);
	bool operator>(const SuggestItem& right);
	int GetHashCode();
	string ToString();
	string term;
	int distance;
	unsigned long long int count;

};

