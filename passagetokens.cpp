#include <string>
#include "passagetokens.h"

using namespace std;
//Constructor to build the passage token with the appropriate title and corresponding passage.
PassageToken::PassageToken(string text, string title){
    passage = text;
    name = title;
}

PassageToken::PassageToken(const PassageToken& cpy){
    this->name = cpy.name;
    this->passage = cpy.passage;
}
