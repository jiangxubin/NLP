#include "SuggestionStage.h"
#include <string>
#include <vector>
#include <unordered_map>


using namespace std;

SuggestionStage::SuggestionStage()
{
}

SuggestionStage::SuggestionStage(int initialCapacity)
{
	Deletes = unordered_map<int, Entry>(initialCapacity);
	//ChunkArray<Node> temp(initialCapacity * 2);
	//Nodes = temp;
	Nodes = ChunkArray<Node>(initialCapacity * 2);
	//cout << "Initial count "<<Nodes.Count << endl;
}


SuggestionStage::~SuggestionStage()
{
}

int SuggestionStage::DeleteCount()
{
	return	Deletes.size();
}

int SuggestionStage::NodeCount()
{
	return Nodes.Count;
}

void SuggestionStage::Clear()
{
	Deletes.clear();
	Nodes.Clear();
}

void SuggestionStage::Add(int deleteHash, string suggestion)
{
	unordered_map<int, Entry>::iterator iter= Deletes.find(deleteHash);
	Entry entry;
	if (iter != Deletes.end()) {
		entry =iter->second;
	}
	else {
		entry = Entry(0,-1);
	}
	int next = entry.first;
	entry.count++;
	entry.first = Nodes.Count;
	Deletes[deleteHash] = entry;
	//cout << "hash " << deleteHash << " entry first " << Deletes[deleteHash].first << " entry count " << Deletes[deleteHash].count << endl;
	Nodes.Add(Node(suggestion, next));
}

void SuggestionStage::CommitTo(unordered_map<int, vector<string>> *permanentDeletes)
{
	//cout << "Begin push hash and str to deletes" << endl;
	unordered_map<int, Entry>::iterator iter_Delete;
	unordered_map<int, vector<string>>::iterator iter_in;
	for(iter_Delete =Deletes.begin(); iter_Delete !=Deletes.end(); iter_Delete++)
	{
		//cout << "permanentdeletes size " << (*permanentDeletes).size() << endl;
		int i;
		int key = iter_Delete->first;
		//cout << "Key " << key << endl;
		Entry value = iter_Delete->second;
		//cout << "term first " << value.first << " term count " << endl;
		vector<string> suggestions;
		iter_in = (*permanentDeletes).find(key);
		if (iter_in!=(*permanentDeletes).end()) {
			suggestions = iter_in->second;
			//cout << "suggestions found " << suggestions[0] << endl;
			i = suggestions.size();
			vector<string> newSuggestions(i+value.count);
			for (int j = 0; j < i; j++) {
				newSuggestions.push_back(suggestions[j]);
			}
			(*permanentDeletes)[key] = newSuggestions;
			suggestions = newSuggestions;
		}
		else {
			//cout << "Suggestions not found" << endl;
			i = 0;
			suggestions = vector< string>(value.count);
			(*permanentDeletes)[key] = suggestions;
			//cout << "key "<<key <<" suggestions[0] " << (*permanentDeletes)[key][0] << endl;
		}
		int next = value.first;
		Node node;
		while (next >= 0) {
			node = Nodes.getValues(next);
			//cout << "Before assign value to prmanentdeletes " << (*permanentDeletes)[key][i] << endl;
			//suggestions[i] = node.suggestion;
			(*permanentDeletes)[key][i] = node.suggestion;
			//cout << "suggestions[i] " << suggestions[i] << endl;
			//cout << "After assign value to prmanentdeletes " << (*permanentDeletes)[key][i] << endl;
			next = node.next;
			i++;
		}
	}
}

SuggestionStage::Node::Node()
{
}

SuggestionStage::Node::Node(string suggestion, int next) {
	this->suggestion = suggestion;
	this->next = next;
}

SuggestionStage::Entry::Entry()
{
}

SuggestionStage::Entry::Entry(int count, int first) {
	this->count = count;
	this->first = first;
}