#ifndef _PASSAGE_H
#define _PASSAGE_H

#include "passagetokens.h"
#include "parttokens.h"
#include <sstream>
#include <vector>
#include <unordered_map>

class Passage: public PassageToken{
private:
 vector<PartToken> passageParts;
 ostringstream buildPassage;
 unordered_map<string, string> passageList;
 bool gotoTripped;
 string nextPassage;
 bool linkExists;
public:
 Passage(string newName, string newPassage);
 Passage(const Passage&);
 void tokenizePrintBlock(string block, unordered_map<string,string>& blockParent);
 void print(unordered_map<string,string>& parent);
 void executeCommand(string commmand);
 bool hasLink(){return linkExists;}
 string getNextPassage() const {return nextPassage;}
};

#endif