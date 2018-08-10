#include "VTM.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;

VTM::VTM()
{
	this->total_count = 1024908267229;
	this->max_dict_word_length = 0;
}


VTM::~VTM()
{
}

bool VTM::load_dict(string corpus_path)
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
			long long int num;
			stringstream ss(count);
			ss >> num;
			if (word.length() > max_dict_word_length) max_dict_word_length = word.length();
			this->dictionary[word] = num;
		}
		//cout << "Dict loaded" << endl;
		return true;
	}
}

void VTM::segment(string input, string *segmented_string, double &probability)
{
	segment(input, segmented_string, probability, max_dict_word_length);
}

void VTM::segment(string input, string *segmented_string, double & probability, int max_dict_word_length)
{
	int cmp = 0x3f;
	int array_size = min(max_dict_word_length, (int)input.length());
    //cout <<"array_size "<<array_size << endl;
	int array_width = ((input.length() - 1) >> 6) + 1;
	//cout << "array_width "<<array_width << endl;
	int array_width_byte = array_width << 3;
	//cout << "array_width_byte "<<array_width_byte << endl;
	vector<vector<unsigned long long int>> segmented_space_bits(array_size, vector<unsigned long long int>(array_width, 0));
	vector<double> log_prob_sum(array_size);
	int circular_index = -1;
	//cout << "circular_index "<<circular_index << endl;

	for (int j = 0; j < input.length(); j++) {
		int space_index = (j - 1) >> 6;
		//cout << "space_index" << space_index << endl;
		int array_copy_byte = min(((space_index + 1) << 3), array_width_byte);
		//cout << "array_copy_byte" << array_copy_byte << endl;
		if (j > 0) {
			///cout << "segmented_space_bits[circular_index][space_index] before " << segmented_space_bits[circular_index][space_index] << endl;
			//cout << "j-1 " << j - 1<< endl;
			//cout << "((j - 1) & cmp) " << ((j - 1) & cmp)<<endl;
			//cout << "(1 << ((j - 1) & cmp)) " << (1 << ((j - 1) & cmp)) << endl;
			//cout << "(long long int)(1 << ((j - 1) & cmp)) " << ((long long int)1 << ((j - 1) & cmp)) << endl;
			segmented_space_bits[circular_index][space_index] |= ((unsigned long long int)1 << ((j - 1) & cmp));
			//cout << "segmented_space_bits[circular_index][space_index] after " << segmented_space_bits[circular_index][space_index] << endl;
		}

		int imax = min(((int)input.length() - j), max_dict_word_length);
		//cout << "imax " << imax << endl;
		for (int i = 1; i <= imax; i++) {
			int destination_index = ((i + circular_index) % array_size);
			//cout << "destination_index " << destination_index << endl;
			string part = input.substr(j, i);
	        //cout << "part " << part << endl;
			double log_prob_part = 0;
			dict_iter = dictionary.find(part);
			if (dict_iter != dictionary.end()) {
				unsigned long long int word_count = dict_iter->second;
				log_prob_part = log10((double)word_count / (double)total_count);
				//cout << "log prob part "<< log_prob_part << endl;
			}
			else {
				log_prob_part = log10(10.0 / (double)(total_count*pow(10, part.length())));
				//cout << "log prob part " << log_prob_part << endl;
			}
			if (j == 0) {
				log_prob_sum[destination_index] = log_prob_part;
				//cout << "log_prob_sum[destination_index] " << log_prob_sum[destination_index] << endl;
			}
			else if (i == max_dict_word_length || log_prob_sum[destination_index] < (log_prob_sum[circular_index] + log_prob_part)) {
				memcpy(&(segmented_space_bits[destination_index][0]), &(segmented_space_bits[circular_index][0]), array_copy_byte);
				//cout << "segmented_space_bits[destination_index][0] " << segmented_space_bits[destination_index][0] << endl;
				//cout << "segmented_space_bits[destination_index][array_width] " << segmented_space_bits[destination_index][array_width-1] << endl;
				log_prob_sum[destination_index] = log_prob_sum[circular_index] + log_prob_part;
				//cout << "log_prob_sum[destination_index] " << log_prob_sum[destination_index] << endl;
			}
		}
		circular_index++;
		//cout << "circular_index "<<circular_index << endl;
		if (circular_index == array_size) {
			circular_index = 0;
			//cout << "circular_index == array_size " << circular_index << endl;
		};
	}
	stringstream ss("");
	int last = -1;
	for (int i = 0; i <= input.length() - 2; i++) {
		//Here we must manually point out that the type of int must be unsigned long long int which equals to no symbol 64 bit int
		if ((segmented_space_bits[circular_index][i >> 6] & ((unsigned long long int)1 << (i&cmp)))>0) {
			//cout << "substr "<<input.substr(last + 1, i - last) << endl;
			ss << input.substr(last + 1, i - last) << " ";
			last = i;
			//cout <<"i " << i << endl;
		}
	}
	ss << input.substr(last + 1);
	string segmented = ss.str();
	//cout << "segmented " << segmented << ",log_prob_sum[circular_index] " << log_prob_sum[circular_index] << endl;
	*segmented_string = segmented;
	probability = log_prob_sum[circular_index];
}
