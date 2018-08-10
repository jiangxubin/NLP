// Segment.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include "VTM.hpp"
#include "EditDistance.hpp"
#include <vector>
#include "SuggestItem.hpp"
#include "ChunkArray.hpp"
#include "SuggestionStage.hpp"
#include "SymSpellCheck.hpp"
#include "Transfer.hpp"

using namespace std;

string Segment(string input);
vector<string> SpellSuggest(string input);

int main()
{

    cout <<"Spell suggest demo" <<endl;
    vector<string> spell_suggest_result= SpellSuggest("FOOTBOLL");
    vector<string> spell_suggest_result_1= SpellSuggest("Footboll");
    vector<string> spell_suggest_result_2= SpellSuggest("bootboll");
    vector<string> spell_suggest_result_3= SpellSuggest("footBoll");
    for(auto iter=spell_suggest_result.begin(); iter!=spell_suggest_result.end();iter++){
        cout << *iter<<endl;
    }

    for(auto iter=spell_suggest_result_1.begin(); iter!=spell_suggest_result_1.end();iter++){
        cout << *iter<<endl;
    }

    for(auto iter=spell_suggest_result_2.begin(); iter!=spell_suggest_result_2.end();iter++){
        cout << *iter<<endl;
    }
    for(auto iter=spell_suggest_result_3.begin(); iter!=spell_suggest_result_3.end();iter++){
        cout << *iter<<endl;
    }

    cout << "Segmentation demo" << endl;
    string result = Segment("iItwasabrightcoldDayinaprilandtheclockswereStrikingthirteen");
    cout << result<<endl;
    return 0;
}

string Segment(string input){
    string path = "../src/new.txt";
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
    //string path = "../src/new.txt";
    //symSpell.ExpandDictionary(path);
    string exp_path = "../src/expanded.txt";
    if (!symSpell.LoadDictionary(exp_path)) {
        cout << "File not found" << endl;
    }
    int upper_count = 0;
    int character_count = 0;
    for(auto iter=input.begin(); iter!=input.end();iter++){
        if((*iter>=65&&*iter<=90)||(*iter>=97&&*iter<=122)){
            character_count+=1;
            if(isupper(*iter)){
                upper_count++;
            }
        }
    }
    if((character_count==upper_count)||(upper_count==1&&isupper(input.at(0)))){
        vector<string> result;
        vector<SuggestItem> res = symSpell.Lookup(input, verbosity);
        for(auto iter(res.begin()); iter!=res.end();iter++)
        {
            result.push_back((*iter).term);
        }
        return result;
    }
    else{
        Transfer trf;
        string input_low=trf.lower(input);
        vector<string> result;
        vector<SuggestItem> res = symSpell.Lookup(input_low, verbosity);
        for(auto iter(res.begin()); iter!=res.end();iter++)
        {
            result.push_back((*iter).term);
        }
        return result;
    }
}
