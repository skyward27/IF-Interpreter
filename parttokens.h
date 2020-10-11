#ifndef PARTTOKENS_H_
#define PARTTOKENS_H_
#include <string>

using namespace std;

enum parttype_t{LINK, GOTO, SET, IF, ELSEIF, ELSE, BLOCK, TEXT, NONE};
//The part token class contains the information needed for each part token. It is a simple class so all functions are included in the header.
class PartToken{
    private:
        parttype_t part;
        string text;
    public:
        PartToken(parttype_t type = NONE , string passage = ""){part = type; text = passage;}
        parttype_t getType() const {return part;}
        string getText() const {return text;}
};

#endif