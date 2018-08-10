#include "DP.h"
#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

DP::DP()
{
	this->max_dict_word_length = 20;
	this->total_count = 1024908267229;
}


DP::~DP()
{
}


bool DP::load_dict(string corpus_path)
{
	ifstream file(corpus_path);
	if (file.good()==0) 
	{
		cout << "Word count dict not found" << endl; 
			return false;
	}
	else 
	{
		string word;
		string count;
		while (file >> word >> count) 
		{
			float num;
			stringstream ss(count);
			ss >> num;
			this->dictionary[word] = num;
		}
		cout << "Dict loaded" << endl;
	}
}

tuple<string, float> DP::segment(string input, int max_dict_word_length)
{
	cache_iter = cache.find(input);
	tuple<string, float> best_composition;
	if (cache_iter != cache.end()) 
	//if (cache.count(input)) 
	{
		best_composition = cache_iter->second;
		//best_composition = cache[input];
		return best_composition;
	}
	else
	{
		best_composition = {"", 0};
	}

	for (int i = 1; i <= min(max_dict_word_length, input.size()); i++) 
	{
		string part = input.substr(0, i);
		float log_porb_part = 0;
		dict_iter = dictionary.find(part);

		if (dict_iter != dictionary.end()) 
		//if (dictionary.count(part)) 
		{
			float word_count = dict_iter->second;
			//long float word_count = dictionary[part];
			log_porb_part = log10(word_count / this->total_count);
		}
		else
		{
			log_porb_part = log10(10.0 / (this->total_count*pow(10.0, part.size())));
		}

		tuple<string, float> rem = {"", 0};

		if(part.size()<input.size())
		{
			rem = segment(input.substr(i), max_dict_word_length);
		}

		if((i==1)||((log_porb_part+get<1>(rem))>get<1>(best_composition)))
		{
			if (part.size() == input.size())
			{
				best_composition = { part, log_porb_part };
			}
			else
			{
				best_composition = { part + " " + get<0>(rem), log_porb_part + get<1>(rem) };
			}
		}
	}
	cache[input] = best_composition;
	return best_composition;
}

int DP::min(int x, int y)
{
	return x<y?x:y;
}

