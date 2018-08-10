//
// Created by root on 8/8/18.
//

#ifndef LINUX_OCR_CAPITALIZATION_H
#define LINUX_OCR_CAPITALIZATION_H

#endif //LINUX_OCR_CAPITALIZATION_H

#include <vector>
#include <string>
using namespace std;

class Transfer
{
public:
    vector<bool> condition;
    Transfer();
    ~Transfer();
    string upper(string input);
    string lower(string input);
};
