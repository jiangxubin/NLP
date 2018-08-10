#include "SuggestItem.h"
#include <sstream>


SuggestItem::SuggestItem()
{
}

SuggestItem::SuggestItem(string term, int distance, unsigned long long int count)
{
	this->term = term;
	this->distance = distance;
	this->count = count;
}


SuggestItem::~SuggestItem()
{
}

int SuggestItem::CompareTo(SuggestItem other)
{
	if (this->distance == other.distance) {
		return (abs((int)this->count-(int)other.count));
	}
	else {
		return (abs(this->distance-other.distance));
	}
}


bool SuggestItem::operator==(SuggestItem other)
{
	if (this->term == other.term && this->distance==other.distance && this->count == other.count) {
		return true;
	}
	else return false;
}

bool SuggestItem::operator>(const SuggestItem & right)
{
	if (this->distance == right.distance) return this->count>right.count;
	return this->distance>right.distance;
}



int SuggestItem::GetHashCode()
{
	hash<string> str_hash;
	size_t hash_code = str_hash(this->term);
	return hash_code;
}

string SuggestItem::ToString()
{
	stringstream ss;
	ss << "{" << this->term << ", " << this->distance << ", " << this->count << "}";
	return ss.str();
}

