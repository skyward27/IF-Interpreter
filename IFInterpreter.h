#ifndef _IFINTERPRETER_H
#define _IFINTERPRETER_H

#include "storytokenizer.h"
#include "Passage.h"
#include "parttokens.h"
#include "passagetokens.h"
#include "passagetokenizer.h"
#include <unordered_map>
#include <string>

class IFInterpreter{
protected:
 vector<Passage> storyPassages;
 unordered_map<string, string> variableMap;
 int currentpassage;
 string story;
public:
 IFInterpreter(string inputfile);
 void play();
 void addVariables();
 void setVariables(string name, string value);
 int findPassage(string passageName);
 string getVariableValue(string name) const;
};

#endif