#include <iostream>
#include <vector>
#include <string>
#include "storytokenizer.h"
#include "passagetokens.h"
#include "parttokens.h"
#include "passagetokenizer.h"

using namespace std;
/*The story tokenizer object contains the code for building a vector of passages that will hold all of the passages of the html story. The main function here is the
constructor that builds the vector. It first replaces all the special html characters with their respective ASCII characters. It then starts tokenizing the story 
by finding the specific strings related to the beginning of the name of the passage as well as the start of the actual passage. It finally builds a member variable
that will be used for iterating through the different members of the vector.*/
StoryTokenizer::StoryTokenizer(string story){
    int namePosStart, namePosEnd, passPosStart, passPosEnd, nextPassStart=0;
    string getName, getPassage;
    int replacePos=0;
    string toReplace("&#39;");
    while(story.find(toReplace,replacePos) != string::npos){
        replacePos = story.find(toReplace, replacePos);
        story.replace(replacePos,toReplace.length(),"'");
        replacePos+=toReplace.length();
    }
    while(story.find(passStartText,nextPassStart)!=string::npos){
        passPosStart = story.find(passStartText,nextPassStart);
        namePosStart = story.find(nameStartText,passPosStart);
        namePosStart = story.find("\"",namePosStart)+1;
        namePosEnd = story.find("\"",namePosStart);
        passPosStart = story.find(">",passPosStart)+1;
        passPosEnd = story.find(passEndText,passPosStart);
        getPassage.assign(story,passPosStart,passPosEnd-passPosStart);
        getName.assign(story,namePosStart,namePosEnd-namePosStart);
        PassageToken newPassage(getPassage,getName);
        passageList.push_back(newPassage);
        nextPassStart=story.find(passStartText,passPosEnd);
    }
    currentPassage = 0;
    totalPassage = passageList.size();
}
//hasNextPassage is just a simple check to make sure when iterating through the vector that the vector does not go through out of bounds
bool StoryTokenizer::hasNextPassage(){
    if(currentPassage>=totalPassage){
        return false;
    }
    else{
        return true;
    }
}
//A simple function for iterating through the vector and returning the current vector item.
PassageToken StoryTokenizer::nextPassage(){
    currentPassage++;
    return passageList[currentPassage-1];
}