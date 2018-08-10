// Segment.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include "VTM.h"
#include "EditDistance.h"
#include <vector>
#include "SuggestItem.h"
#include "ChunkArray.h"
#include "SuggestionStage.h"
#include "SymSpellCheck.h"
#include "Transfer.h"

using namespace std;

string Segment(string input);
vector<string> SpellSuggest(string input);

int main()
{

    vector<string> spell_suggest_result= SpellSuggest("footboll");
    for(auto iter=spell_suggest_result.begin(); iter!=spell_suggest_result.end();iter++){
        cout << *iter<<endl;
    }

    string result = Segment("iItwasabrightcoldDayinaprilandtheclockswereStrikingthirteen");
    cout << result<<endl;
    return 0;
}

string Segment(string input){
    string path = "/home/augustus/Documents/NLP/src/new.txt";
    VTM seg;
    Transfer trf;
    string segmented;
    double prob=0.0;
    seg.load_dict(path);
    string test_seg = trf.lower(input);
    seg.segment(test_seg, &segmented, prob);
    string res = trf.upper(segmented);
    return res;
}

vector<string> SpellSuggest(string input){
    int maxEditDistance = 2;
    SymSpellCheck::Verbosity verbosity = SymSpellCheck::All;
    SymSpellCheck symSpell(-1, maxEditDistance, -1,10);
    string path = "/home/augustus/Documents/NLP/src/new.txt";
    if (!symSpell.LoadDictionary(path)) {
        cout << "File not found" << endl;
    }
    Transfer trf;
    string input_low=trf.lower(input);
    vector<string> result;
    vector<SuggestItem> res = symSpell.Lookup(input_low, verbosity);
    for(auto iter(res.begin()); iter!=res.end();iter++)
    {
//        cout << "Suggestion "<<iter->term<<endl;
//        cout << "Editdistance " << iter->distance<<endl;
        result.push_back((*iter).term);
    }
    return result;
}
