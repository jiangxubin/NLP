#include "TM.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

TM::TM()
{
	this->total_count = 1024908267229;
	this->max_dict_word_length = 20;
}


TM::~TM()
{
}

bool TM::load_dict(string corpus_path)
{
	ifstream file(corpus_path);
	if (file.good() == 0)
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
			double num;
			stringstream ss(count);
			ss >> num;
			if (word.length() > max_dict_word_length) max_dict_word_length = word.length();
			this->dictionary[word] = num;
		}
		cout << "Dict loaded" << endl;
	}
}

tuple<string, double> TM::segment(string input)
{
	return segment(input, max_dict_word_length);
}

tuple<string, double>TM::segment(string input, int max_dict_word_length)
{
	int cmp = 0x3f;
	int array_size = min(max_dict_word_length,(int)input.length());
	//cout << array_size << endl;
	int array_width = ((input.length()-1)>>6)+1;
	//cout << array_width << endl;
	int array_width_byte = array_width << 3;
	//cout << array_width_byte << endl;
	int **segmented_space_bits = new int *[array_size];
	segmented_space_bits[0] = new int(array_size*array_width);
	for (int m = 1; m < array_size; m++) {
		segmented_space_bits[m] = segmented_space_bits[0] + m*array_width;
	}
	for (int i = 0; i < array_size; i++) {
		for (int j = 0; j < array_width; j++) {
			segmented_space_bits[i][j] = 0;
		}
	}
	//for (int i = 0; i < array_size; i++) {
	//	for (int j = 0; j < array_width; j++) {
	//		cout << segmented_space_bits[i][j]<<endl;
	//	}
	//}

	double *log_prob_sum = new double [array_size];
	int circular_index = -1;
	cout << circular_index << endl;
	
	for (int j = 0; j < input.length(); j++) {
		int space_index = (j - 1) >> 6;
		cout << "space_index" << space_index<< endl;
		int array_copy_byte = min(((space_index + 1) << 3), array_width_byte);
		cout << "array_copy_byte"<< array_copy_byte << endl;
		if (j > 0) { 
			// origin = segmented_space_bits[circular_index][space_index];
			//int des = (1 << ((j - 1) & cmp));
			segmented_space_bits[circular_index][space_index] |= (1 << ((j - 1) & cmp));
			//segmented_space_bits[circular_index][space_index] = origin | des;
		}

		int imax = min(((int)input.length() - j), max_dict_word_length);
		for (int i = 1; i <= imax; i++) {
			int destination_index = ((i + circular_index) % array_size);
			string part = input.substr(j, i);
			double log_prob_part = 0;
			dict_iter = dictionary.find(part);
			if (dict_iter != dictionary.end()) {
				double word_count = dict_iter->second;
				log_prob_part = log10((double)word_count / (double)total_count);
			}
			else {
				log_prob_part = log10(10.0 / (double)(total_count*pow(10, part.length())));
			}
			if(j == 0) {
				log_prob_sum[destination_index] = log_prob_part;
			}
			else if (i == max_dict_word_length || log_prob_sum[destination_index] < (log_prob_sum[circular_index] + log_prob_part)) {
				memcpy(segmented_space_bits + destination_index*array_width, segmented_space_bits + circular_index*array_width, array_copy_byte);
				log_prob_sum[destination_index] = log_prob_sum[circular_index] + log_prob_part;
			}
		}
		circular_index++;
		if (circular_index == array_size) { 
			circular_index = 0;
		};
	}
	stringstream ss("");
	int last = -1;
	for (int i = 0; i < input.length() - 2; i++) {
		if ((segmented_space_bits[circular_index][i>>6]&(1<<(i&cmp)))>0) {
			ss << input.substr(last + 1, i - last) << " ";
			last = i;
			cout << i << endl;
		}
	}
	ss << input.substr(last + 1);
	string segmented = ss.str();
	//delete []segmented_space_bits[0];
	//delete []segmented_space_bits;
	//delete []log_prob_sum;
	return {segmented, log_prob_sum[circular_index]};
}
