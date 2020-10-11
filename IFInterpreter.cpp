#include "IFInterpreter.h"
#include <fstream>
#include <iostream>

IFInterpreter::IFInterpreter(string inputFile){
    string line, story;
    ifstream in(inputFile);
    getline(in, inputFile);
    while(in && line != "</html>"){
        story += line + '\n';
        getline(in, line);
    }

    StoryTokenizer newStoryTokens(story);
    this->story = story;
    while(newStoryTokens.hasNextPassage()){
        PassageToken temp(newStoryTokens.nextPassage());
        Passage temp2(temp.getName(),temp.getText());
        storyPassages.push_back(temp2);
    }
    currentpassage = 0;
    addVariables();
}
//Simple function to add variables from the story to the variable map.
void IFInterpreter::addVariables(){
    string variable, svalue;
    int pos(0),end(0);
    pos = story.find("(set: $",pos);
    while(pos != string::npos&& pos < story.size()){
        pos = story.find('$',pos)+1;
        end = story.find(' ',pos);
        variable = story.substr(pos, end-pos);
        pos = end+4;
        end = story.find(')', pos);
        svalue = story.substr(pos, end-pos);
        variableMap.emplace(variable,svalue);
        pos = end;
        pos = story.find("(set: $",pos);
    }
}

void IFInterpreter::setVariables(string name, string value){
    variableMap.at(name) = value;
}

string IFInterpreter::getVariableValue(string name) const{
    return variableMap.at(name);
}

int IFInterpreter::findPassage(string passageName){
    int passageNumber;
    for(int i = 0;i<storyPassages.size();i++){
        if(storyPassages.at(i).getName() == passageName){
            passageNumber = i;
        }
    }
    return passageNumber;
}
//The play function handles the execution of the story. It continues until it reaches a passage without a link.
void IFInterpreter::play(){
    bool more=1;
    currentpassage = 0;
    while(more){
        cout << "Passage: " << storyPassages[currentpassage].getName() << endl;
        storyPassages.at(currentpassage).print(variableMap);
        more = storyPassages.at(currentpassage).hasLink();

        currentpassage = findPassage(storyPassages[currentpassage].getNextPassage());
        //currentpassage++;
    }
}