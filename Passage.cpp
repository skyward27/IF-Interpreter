#include "Passage.h"
#include "passagetokenizer.h"
#include <vector>
#include <iostream>

Passage::Passage(string newName, string newPassage){

    name = newName;
    passage = newPassage;
    PassageTokenizer temp(passage);
    while(temp.hasNextPart()){
        passageParts.push_back(temp.nextPart());
    }
    gotoTripped = false;
}

Passage::Passage(const Passage& cpy){
    this->name = cpy.name;
    this->passage = cpy.passage;
    this->passageParts = cpy.passageParts;
}
/*The print functions iterates through the various parts of the passage and executes them according to the design specification. It then ends by asking the user to pick
a link and updating its next passage variable to be whatever that link is. */
void Passage::print(unordered_map<string,string>& parent){
    passageList.clear();
    buildPassage.str("");
    int varstart =0, varend =0, valstart =0, valend = 0;
    string variable, value;
    int passageChoice;
    bool executeBlock(false), doElse(false);
    linkExists = false;
    gotoTripped = false;
    for (int i = 0; i < passageParts.size();i++){
        string text = passageParts[i].getText();
        if(passageParts[i].getType() == LINK){
            doElse =false;
            valstart = text.find("&gt;");
            if(valstart == string::npos){
                variable = value = text.substr(2,text.size()-4);
            }
            else{
                varend = valstart;
                valstart = valstart + 4;
                valend = text.find("]", valend);
                varstart = text.find('[' +2);
                variable = text.substr(valstart, valend-valstart);
                value = text.substr(varstart,varend-varstart);
            }
            linkExists = true;
            buildPassage << variable;
            passageList.emplace(variable,value);
            //cout << "Link: display=" << variable <<", target=" << value <<"\n";
        }
        if(passageParts[i].getType() == GOTO){
            doElse = false;
            varstart = text.find("&quot;")+5;
            varend = text.find('&',varstart);
            variable = text.substr(varstart,varend-varstart);
            nextPassage = variable;
            cout << buildPassage.str() << endl << endl;
            return;
            //cout << "Go-to: target=" << passageParts[i].getText() <<"\n";
        }
        if(passageParts[i].getType() == SET){
            doElse = false;
            valstart = text.find("to ")+3;
            valend = text.find(')',valstart);
            varstart = text.find('$')+1;
            varend = valstart -4;
            variable = text.substr(varstart, varend-varstart);
            value = text.substr(valstart,valend-valstart);
            parent.at(variable) = value;
            
            //cout << "Set: var=$" << variable <<", value="<< value <<"\n";
        }
        if(passageParts[i].getType() == IF){
            doElse = false;
            valstart = text.find("is ")+3;
            valend = text.find(')',valstart);
            varstart = text.find('$')+1;
            varend = valstart -4;
            variable = text.substr(varstart, varend-varstart);
            value = text.substr(valstart,valend-valstart);
            if(parent.at(variable) == value){
                executeBlock = true;
            }
            else
            {
                executeBlock = false;
                doElse = true;
            }
            //cout << "If: var=$" << variable<<", value="<< value <<"\n";
        }
        if(passageParts[i].getType() == ELSEIF){
            doElse = false;
            valstart = text.find("is ")+3;
            valend = text.find(')',valstart);
            varstart = text.find('$')+1;
            varend = valstart -4;
            variable = text.substr(varstart, varend-varstart);
            value = text.substr(valstart,valend-valstart);
            if(parent.at(variable) == value){
                executeBlock = true;
            }
            else
            {
                executeBlock = false;
                doElse= true;
            }
            
           // cout << "Else-if: var=$" << variable << ", value="<< value << "\n";
        }
        if(passageParts[i].getType() == ELSE){
            if(doElse == true){
                executeBlock = true;
            } 
            //cout << "Else:" << endl;
        }
        if(passageParts[i].getType() == BLOCK){
            if(executeBlock == true){
                tokenizePrintBlock(passageParts[i].getText().substr(1,passageParts[i].getText().size()-2),parent);
                executeBlock = false;
            }

        }
        if(passageParts[i].getType() == TEXT){
            buildPassage << passageParts[i].getText() << " ";
            //cout << "\"" << passageParts[i].getText() << "\"\n";
        }
    }
    cout << buildPassage.str() << endl << endl;
    if(gotoTripped == true){
        return;
    }
    if(passageList.size() > 0){
        cout << "Please choose the number of the link you wish to select:\n";
        int i = 0;
        for(auto& x:passageList){
            cout << i+1 << ". " << x.second << endl;
            i++;
        }
        cin >> passageChoice;
        if(cin.fail()||passageChoice <=0||passageChoice > passageList.size()){
            cout << "Invalid selection. Please choose again:\n";
            cin >> passageChoice;
        }
        auto it = passageList.begin();
        advance(it,passageChoice-1);
        nextPassage = it->second;
    }
}

/*This function takes in block tokens and attempts to tokenize then execute them in the same method that the print function above executes parts. */
void Passage::tokenizePrintBlock(string block, unordered_map<string,string>& blockParent){
    int start, partstart=0, partend, passlength;
    bool blockcheck= false,doelse = false, doblock = false;
    bool buildtext = 0;
    int varstart =0, varend =0, valstart =0, valend = 0;
    string variable, value;
    string part;
    parttype_t temptype;
    passlength = block.size();
    start = 0;
    while(start!=passlength){
        partstart = block.find_first_of("[(",start);
        jump:
        if(partstart == string::npos){
            temptype = TEXT;
            part = block.substr(start, passlength-start);
            buildPassage << part;
            //cout << "\"" << part << "\"\n";
            break;
        }
        if(block[partstart] == '[' && block[partstart + 1]=='['){
            doelse = false;
            partend = block.find("]]",partstart);
            temptype = LINK;
            partend++;
            part = block.substr(partstart,partend+1-partstart);
            valstart = part.find("&gt;");
            if(valstart == string::npos){
                variable = value = part.substr(2,part.size()-4);
            }
            else{
                varend = valstart;
                valstart = valstart + 4;
                valend = part.find("]", valend);
                varstart = part.find('[' +2);
                variable = part.substr(valstart, valend-valstart);
                value = part.substr(varstart,varend-varstart);
            }
            buildPassage << variable;
            passageList.emplace(variable,value);
            buildtext = 1;
            linkExists = true;
        }
        
        if(block.substr(partstart, 7)== "(go-to:"){
            doelse = false;
            temptype = GOTO;
            partend = block.find(')',partstart);
            part = block.substr(partstart, partend+1-partstart);
            varstart = part.find("&quot;")+6;
            varend = part.find('&',varstart);
            variable = part.substr(varstart,varend-varstart);
            nextPassage = variable;
            gotoTripped = true;
            buildtext = 1;
        }
        if(block.substr(partstart, 5)== "(set:"){
            doelse = false;
            temptype = SET;
            partend = block.find(')',partstart);
            part = block.substr(partstart, partend+1-partstart);
            valstart = part.find("to ")+3;
            valend = part.find(')',valstart);
            varstart = part.find('$')+1;
            varend = valstart -4;
            variable = part.substr(varstart, varend-varstart);
            value = part.substr(valstart,valend-valstart);
            blockParent.at(variable) = value;
            buildtext = 1;
        }
        if(block.substr(partstart, 4)== "(if:"){
            temptype = IF;
            doelse = false;
            blockcheck = true;
            partend = block.find(')',partstart);
            part = block.substr(partstart, partend+1-partstart);
            valstart = part.find("is ")+3;
            valend = part.find(')',valstart);
            varstart = part.find('$')+1;
            varend = valstart -4;
            variable = part.substr(varstart, varend-varstart);
            value = part.substr(valstart,valend-valstart);
            if(blockParent.at(variable) == value){
                doblock = true;
            }
            else
            {
                doelse = true;
                doblock = false;
            }
            
            buildtext = 1;
        }
        if(block.substr(partstart, 9)== "(else-if:"){
            temptype = ELSEIF;
            doelse = false;
            blockcheck = true;
            partend = block.find(')',partstart);
            part = block.substr(partstart, partend+1-partstart);
            valstart = part.find("is ")+3;
            valend = part.find(')',valstart);
            varstart = part.find('$')+1;
            varend = valstart -4;
            variable = part.substr(varstart, varend-varstart);
            value = part.substr(valstart,valend-valstart);
            if(blockParent.at(variable) == value){
                doblock = true;
            }
            else
            {
                doblock = false;
                doelse = true;
            }
            
            buildtext = 1;
        }
        if(block.substr(partstart, 6)== "(else:"){
            temptype = ELSE;
            blockcheck = true;
            partend = block.find(')',partstart);
            part = block.substr(partstart, partend+1-partstart);
            if(doelse = true){
                doblock = true;
            }
            
            buildtext = 1;
        }
        if(!buildtext){
            partstart = block.find_first_of("([",partstart + 1);
            goto jump;
        }
        if(buildtext){
            if(partstart-start > 0){
                part = block.substr(start,partstart-start-1);
                buildPassage << part;
                //cout << "\"" << part << "\"\n";
            }
            start = partend + 1;
            buildtext = 0;

        }
        /*/switch (temptype){
            case LINK: cout << "Link: display=" << variable <<", target=" << value <<"\n";
            break;
            case GOTO: cout << "Go-to: target=" << variable << "\n";
            break;
            case SET: cout << "Set: var=$" << variable <<", value="<< value << "\n";
            break;
            case IF: cout << "If: var=$" << variable <<", value="<< value << "\n";
            break;
            case ELSEIF: cout << "Else-if: var=$" << variable <<", value="<< value << "\n";
            break;
            case ELSE: cout << "Else:\n";
            break;
        }*/
        temptype = NONE;
        part = "";
        if(blockcheck == true){
            int counter = 0;
            temptype = BLOCK;
            partstart = block.find('[',partstart);
            for(int i = 1; i<passlength;i++){
                if(block[partstart+i]=='[') counter++;
                if(block[partstart+i]==']'&&counter == 0){
                    partend = partstart+i;
                    break;
                }
                if(block[partstart+i] == ']') counter--;
            }
            part = block.substr(partstart, partend+1-partstart);
            start = partend+1;
            if(doblock == true){
                tokenizePrintBlock(part.substr(1,part.size()-2),blockParent);
            }
            doblock = false;
            blockcheck = false;
        }
    }
}