#ifndef _PASSAGETOKENS_H
#define _PASSAGETOKENS_H

#include <string>
using namespace std;

class PassageToken{
    protected:
        string passage;
        string name;
    public:
        PassageToken(string text = "", string title = "");
        PassageToken(const PassageToken&);
        string getText() const {return passage;}
        string getName() const {return name;}
};

#endif