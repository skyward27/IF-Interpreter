#ifndef _STORYTOKENIZER_H
#define _STORYTOKENIZER_H
#include <string>
#include <vector>
#include "passagetokens.h"
#include "parttokens.h"
#include "passagetokenizer.h"
using namespace std;

class StoryTokenizer{
    private:
        vector<PassageToken> passageList;
        int currentPassage, totalPassage;
        const string passStartText = "<tw-passagedata";
        const string nameStartText = " name=\"";
        const string passEndText = "</tw-passagedata>";
    public:
        StoryTokenizer(string story = "");
        bool hasNextPassage();
        PassageToken nextPassage();
};

#endif