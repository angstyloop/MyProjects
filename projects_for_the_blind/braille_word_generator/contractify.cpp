// Write a function that takes a word and returns
//  a list of all the possible formations of the
//  word using braille contractions.
//
//  Is a recursive solution possible? --> Yes.

// these are included in possibles.cpp. take these out after writing makefile.
//#include<list>
//#include<string>
//#include<iostream>

// take these out after writing makefile.
#include<map>
#include<utility>
#include<vector>
#include "./possibles.cpp"

//define aliases
using s_t = std::string;                            //string
using v_t = std::vector<std::string>;               //string vector
using vl_t = std::list<std::vector<std::string>>;   //vector list
using sl_t = std::list<std::string>;                //string list


// Determine whether the smaller of two strings matches the beginning
//  of the larger string. 
//  e.g. match("abc","ab") --> true
//       match("ab","abc") --> true  (order doesnt matter)
//       match("abc","b")  --> false

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

// recursive helper function for contractify(). starts at the head of
//  the string. uses current character to get a list of contractions
//  that might start there. if any contraction matches starting at that
//  character a recursive call to contractify is made, using the substring
//  formed by removing the matching contraction from the head of the string.
//  A memo, in the form of a vector of strings (contractions), is used
//  to store the partitioning so far. A queue is passed along for
//  storing the memos whenever the string argument to helper is empty.
void helper( vl_t & Q, v_t memo, s_t str ) {

    // a list to store possible contractions
    sl_t list;

    //base case. if input string is empty, then memo must be a full
    //  possible partitioning of str. stick it in the queue.
    if (str == "") 
        Q.push_back(memo);

    else {
        // get a list of possible contractions
        list = possibles(str[0]);

        // loop through the contractions in list
        for ( s_t contr : list)

            // if the contraction x fails to match
            //  or is too long for what's left of
            //  the string, helper isn't called.
            if (contr.size() <= str.size() && match(contr, str)) {

                // push the contraction contr into the string vector
                //  memo representing the partitioned word so far.
                memo.push_back(contr);

                // call helper recursively with the new memo,
                //  and the substring formed by removing contr
                //  from the beginning of the word string str.
                helper( Q
                ,   memo
                ,   str.substr(contr.size(), str.size()-contr.size())
                );
            }

    }
}

// given a word, return a list of vectors of strings corresponding
//  to all the possible partitionings of the word using the single-
//  character contractions listed in possibles.cpp.
vl_t contractify ( s_t word ){
    vl_t Q;
    v_t memo;
    helper (Q, memo, word);
    return Q; 
}

// a test program for contractify(). does it work? --> yes!
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

