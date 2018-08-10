#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum DistanceAlgorithm
{
	Damerau
};

class EditDistance
{
public:
	//enum DistanceAlgorithm
	//{
	//	Damerau
	//};
	EditDistance();
	EditDistance(string baseString, DistanceAlgorithm algorithm);
	~EditDistance();
	string left;
	string right;
	int compare(string string2, int maxDistance);
	int DamerauLevenshteinDistance(string string2, int maxDistance);
	static int EditDistanceRe(string l, string r);
	static int EditDistanceDp(string left, string right);
	static int EidtDistanceOp(string left, string right);

private:
	string baseString;
	DistanceAlgorithm algorithm;
	vector<int> v0;
	vector<int> v2;
};
