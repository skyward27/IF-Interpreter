#ifndef PASSAGETOKENIZER_H_
#define PASSAGETOKENIZER_H_
#include <string>
#include <vector>
#include "parttokens.h"

using namespace std;

class PassageTokenizer{
    private:
        vector<PartToken> tokenList;
        int currentpart, totalpart;
    public:
        PassageTokenizer(string passage="");
        bool hasNextPart();
        PartToken nextPart();

};

#endif