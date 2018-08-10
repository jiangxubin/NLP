#pragma once
#include <string>
#include <map>
#include <tuple>

using namespace std;
class TM
{
public:
	TM();
	~TM();
	map<string, double> dictionary;
	map<string, double> ::iterator dict_iter;
	int max_dict_word_length;
	double total_count;
	bool load_dict(string corpus_path);
	tuple<string, double> segment(string input);
	tuple<string, double> segment(string input, int max_dict_word_length);
};

