// Write a function that takes a word and returns
//  a list of all the possible formations of the
//  word using braille contractions.
//
//  Is a recursive solution possible?

#include<list>
#include<string>
#include<iostream>
#include<map>
#include<utility>

std::list<std::string> possible (char first_letter) {
    std::map<char, std::list<std::string>> possibles;

    possibles.insert({'a',{"a", "ab", "abc"}});
    possibles.insert({'b',{"b", "bc"}});
    possibles.insert({'c',{"c"}});

    return possibles[first_letter];
}


void
helper( std::list<std::string>& Q
    ,   std::string memo
    ,   std::string str )
{
    if (str == "")
        Q.push_back(memo);

    else {
        for ( auto x : possible(str[0]) )
            helper( Q
                ,   memo+ "("+ x + ")"
                ,   str.substr(x.size(), str.size()-x.size())
            );
    }
}

std::list<std::string> 
contractify ( std::string word ){
    std::list<std::string> Q;
    helper (Q, "", word);
    return Q; 
}

int main() {
    const std::string str = "abc";
    auto Q = contractify(str);
    for (auto x : Q)
        std::cout << x << std::endl;
}

