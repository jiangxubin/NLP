#pragma once
#include <tuple>
#include <iostream>
#include <string>
#include <map>

using namespace std;

class DP
{
public:
	DP();
	~DP();
	map<string, float> dictionary;
	map<string, float>::iterator dict_iter;
	map<string, tuple<string, float>>::iterator cache_iter;
	map<string, tuple<string, float>> cache;
	int max_dict_word_length;
	float total_count;
	bool load_dict(string corpus_path);
	tuple<string, float> segment(string input, int max_dict_word_length);
	int min(int x, int y);
};

