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
#include<vector>
#include "./possibles.cpp"

//define aliases
using s_t = std::string;                            //string
using v_t = std::vector<std::string>;               //string vector
using vl_t = std::list<std::vector<std::string>>;   //vector list
using sl_t = std::list<std::string>;                //string list


bool match(s_t s1, s_t s2) {

    //figure out which string is smaller
    s_t *big_pt, *small_pt; 
    if (s1.size() > s2.size()) {
        big_pt = &s1;
        small_pt = &s2;
    } else {
        big_pt = &s2;
        small_pt = &s1;
    }
    // use references for clarity
    const s_t& big = *big_pt;
    const s_t& small = *small_pt;

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


void helper( vl_t & Q, v_t memo, s_t str ) {
    // list to store possible contractions
    sl_t list;

    if (str == "") {
        //std::cout << "Adding " + memo + "to the queue." 
        //          << std::endl;

        Q.push_back(memo);
    }

    else {
        //std::cout << "Current character: " << str[0] << '\n';
        list = possibles(str[0]);
        for ( s_t x : list)
            // if the contraction x fails to match
            //  or is too long for what's left of
            //  the string, helper isn't called.
            if (x.size() <= str.size() && match(x, str)) {
                //std::cout<< x << " does match."  <<std::endl;
                memo.push_back(x);
                helper( Q
                ,   memo
                ,   str.substr(x.size(), str.size()-x.size())
                );
            }

    }
}

vl_t contractify ( s_t word ){
    vl_t Q;
    v_t memo;
    helper (Q, memo, word);
    return Q; 
}

int main() {
    sl_t str_list = {
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

    for (s_t str : str_list) {
        vl_t Q = contractify(str);
        std::cout << str << ":\n";
        for (v_t vec : Q) {
            for (s_t elem : vec)
                std::cout << elem << " | ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

