//
// Created by root on 8/8/18.
//

#include "Transfer.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

Transfer::Transfer() {

}

Transfer::~Transfer() {

}


//Convert string into lower case and save the index of upper character
string Transfer::lower(string input) {
    vector<char> data(input.begin(), input.end());
    vector<char> lowwer_data;
    for(auto iter=data.begin(); iter!=data.end(); iter++){
        if(*iter>=65 && *iter<=90)
        {
            //cout << "Capital alphabet "<<*iter << endl;
            char low = char((int)(*iter)+32);
            //cout << "Low "<< low<< endl;
            lowwer_data.push_back(low);
            this->condition.push_back(true);
        }
//        else if(*iter>=97 && *iter<=122){
//            cout << "Lower alphabet "<< *iter<< endl;
//        }
//        else if(*iter>=48 && *iter<=57){
//            cout << "Digit " << *iter << endl;
//        }
        else{
            lowwer_data.push_back(*iter);
            this->condition.push_back(false);
            //cout << "Common character" << endl;
        }
    }
    return string(lowwer_data.begin(), lowwer_data.end());
}

//Use saved index of upper character to restore the result of lower case to upper case
string Transfer::upper(string input) {
    auto iter_cond=this->condition.begin();
    vector<char> upper_data;
    for(auto iter=input.begin(); iter!=input.end(); iter++){
        if(isblank(*iter)){
            continue;
        }
        if(*iter_cond){
            *iter = char((int)(*iter)-32);
            //cout << *iter << endl;
        }
        iter_cond++;

    }
    return string(input.begin(), input.end());
}

