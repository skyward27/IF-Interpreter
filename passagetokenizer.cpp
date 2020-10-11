#include "passagetokenizer.h"
#include <iostream>

//The passagetokenizer constructor builds the classes part token vector by looking for specific keywords in the text
//If it can't find any of those keywords it labels that specific part of the fiction as a text token
PassageTokenizer::PassageTokenizer(string passage){
    int start, partstart=0, partend, passlength,newstart;
    bool block= false;
    bool buildtext =0;
    string part;
    parttype_t temptype;
    passlength = passage.size();
    start = newstart =0;
    
    while(start!=passlength){
        partstart = passage.find_first_of("[(",start);
        jump:
        if(partstart == string::npos){
            temptype = TEXT;
            part = passage.substr(start, passlength-start);
            PartToken textendToken(temptype,part);
            tokenList.push_back(textendToken);
            break;
        }
        if(passage[partstart] == '[' && passage[partstart + 1]=='['){
            partend = passage.find("]]",partstart);
            temptype = LINK;
            partend++;
            part = passage.substr(partstart,partend+1-partstart);
            buildtext = 1;
        }
        
        if(passage.substr(partstart, 7)== "(go-to:"){
            temptype = GOTO;
            partend = passage.find(')',partstart);
            part = passage.substr(partstart, partend+1-partstart);
            buildtext = 1;
        }
        if(passage.substr(partstart, 5)== "(set:"){
            temptype = SET;
            partend = passage.find(')',partstart);
            part = passage.substr(partstart, partend+1-partstart);
            buildtext = 1;
        }
        if(passage.substr(partstart, 4)== "(if:"){
            temptype = IF;
            block = true;
            partend = passage.find(')',partstart);
            part = passage.substr(partstart, partend+1-partstart);
            buildtext = 1;
        }
        if(passage.substr(partstart, 9)== "(else-if:"){
            temptype = ELSEIF;
            block = true;
            partend = passage.find(')',partstart);
            part = passage.substr(partstart, partend+1-partstart);
            buildtext = 1;
        }
        if(passage.substr(partstart, 6)== "(else:"){
            temptype = ELSE;
            block = true;
            partend = passage.find(')',partstart);
            part = passage.substr(partstart, partend+1-partstart);
            buildtext = 1;
        }
        if(!buildtext){
            partstart = passage.find_first_of("([",partstart+1);
            goto jump;
        }
        if(buildtext){
            if(partstart - start > 0){
            PartToken textToken(TEXT, passage.substr(start,partstart-start-1));
            tokenList.push_back(textToken);
            }
            start = partend + 1;
            buildtext = 0;
        }
        
        PartToken tempToken(temptype,part);
        tokenList.push_back(tempToken);
        temptype = NONE;
        part = "";
        if(block == true){
            int counter = 0;
            temptype = BLOCK;
            partstart = passage.find('[',partstart);
            for(int i = 1; i<passlength;i++){
                if(passage[partstart+i]=='[') counter++;
                if(passage[partstart+i]==']'&&counter == 0){
                    partend = partstart+i;
                    break;
                }
                if(passage[partstart+i] == ']') counter--;
            }
            part = passage.substr(partstart, partend+1-partstart);
            PartToken blockToken(temptype,part);
            tokenList.push_back(blockToken);
            start = partend+1;
            block = false;
            continue;
        }
    }
    
    currentpart = 0;
    totalpart = tokenList.size();
}

//Using an internal counter, this function allows the class to keep track of how many part it has in total.
bool PassageTokenizer::hasNextPart(){
    if(currentpart>=totalpart) return false;
    else return true;
}
//This function allows the user to iterate through all the part tokens in the class. This is done sequentially as this is all that is needed for the project currently.
PartToken PassageTokenizer::nextPart(){
    currentpart++;
    return tokenList[currentpart - 1];
}