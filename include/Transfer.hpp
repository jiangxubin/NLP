//
// Created by root on 8/8/18.
//

#ifndef __TRANSFER__
#define __TRANSFER__

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
#endif //LINUX_OCR_CAPITALIZATION_H
