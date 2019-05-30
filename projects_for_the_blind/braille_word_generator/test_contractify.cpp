// Write a function that takes a word and returns
//  a list of all the possible formations of the
//  word using braille contractions.
//
//  Is a recursive solution possible?

//#include<list>
//#include<string>
//#include<iostream>
#include<map>
#include<utility>
#include "./possibles.cpp"

bool match(std::string s1, std::string s2) {
    //figure out which string is smaller
    std::string *big_pt, *small_pt; 
    if (s1.size() > s2.size()) {
        big_pt = &s1;
        small_pt = &s2;
    } else {
        big_pt = &s2;
        small_pt = &s1;
    }
    const std::string& big = *big_pt;
    const std::string& small = *small_pt;
    //check every character in small against the
    //  character in big with the same index,
    //  starting at 0. when a character fails
    //  to match, return false.
    for (int i=0; i<small.size(); ++i) {
        if (small[i] != big[i])
            return false;
    }
    //otherwise the match is good
    return true;
}

//std::list<std::string> possible (char first_letter) {
    //std::map<char, std::list<std::string>> possibles;

    //possibles.insert({'a',{"a", "ab", "abc"}});
    //possibles.insert({'b',{"b", "bc"}});
    //possibles.insert({'c',{"c"}});

    //return possibles[first_letter];
    //switch (first_letter) {
        //case 'a':
            //return std::list<std::string> {"a", "ab", "abc"};
        //case 'b':
            //return std::list<std::string> {"b", "bc"};
        //case 'c':
            //return std::list<std::string> {"c"};
        //default:
            //exit(EXIT_FAILURE);

    //}
//}


void
helper( std::list<std::string>& Q
    ,   std::string memo
    ,   std::string str )
{
    std::list<std::string> list;

    if (str == "") {
        //std::cout << "Adding " + memo + "to the queue." 
        //          << std::endl;

        Q.push_back(memo);
    }

    else {
        //std::cout << "Current character: " << str[0] << '\n';
        list = possibles(str[0]);
        for ( auto x : list)
            // if the contraction x fails to match
            //  or is too long for what's left of
            //  the string, helper isn't called.
            if (x.size() <= str.size() && match(x, str)) {
                //std::cout<< x << " does match."  <<std::endl;
                helper( Q
                ,   memo + "(" + x + ")"
                ,   str.substr(x.size(), str.size()-x.size())
                );
            }

    }
}

std::list<std::string> 
contractify ( std::string word ){
    std::list<std::string> Q;
    helper (Q, "", word);
    return Q; 
}

int main() {
    std::list<std::string> str_list = {
        "vegetable"
    ,   "cucumber"
    ,   "hypothesis"
    ,   "butter"
    ,   "monster"
    ,   "ventrical"
    ,   "aviation"
    ,   "nation"
    ,   "zebra"
    };

    std::cout   << 
"\ncontractify(word) should return all possible partitionings\n of word, using only the single-character contractions listed\n in possibles.cpp.\n\n Here are some examples:\n\n" 

                ;

    for (auto str : str_list) {
        auto Q = contractify(str);
        std::cout << str << ":\n";
        for (auto x : Q)
            std::cout << x << std::endl;
        std::cout << std::endl;
    }
}

