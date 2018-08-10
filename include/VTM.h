#pragma once
#include <string>
#include <map>
#include <tuple>

using namespace std;
class VTM
{
public:
	VTM();
	~VTM();
	map<string, unsigned long long int> dictionary;
	map<string, unsigned long long int> ::iterator dict_iter;
	int max_dict_word_length;
	unsigned long long int total_count;
	bool load_dict(string corpus_path);
	void segment(string input, string *segmented_string, double &probability_ref);
	void segment(string input, string *segmented_string, double &probability_ref, int max_dict_word_length);
};

