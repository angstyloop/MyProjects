// Given a lowercase letter character, returns a list of 
//  contractions* starting with that letter.

// Only single character contractions are used. Wordsigns
//  and other rules will be added in later.

#include<list>
#include<string>
#include <iostream>

std::list<std::string> possibles (char first_letter) {
  switch(first_letter) {
    case 'a':
      return std::list<std::string> {
         "a"
      //,  "ab"
      //,  "abv"
      //,  "ac"
      //,  "acr"
      //,  "afn"
      //,  "afw"
      //,  "ag"
      //,  "agst"
      //,  "ally"
      //,  "alm"
      //,  "alr"
      //,  "al"
      //,  "alth"
      //,  "alt"
      //,  "alw"
      ,  "ance"
      ,  "and"
      ,  "ar"
      ,  "as"
      };
    case 'b':
      return std::list<std::string> {
         "b"
      ,  "bb"
      ,  "be"
      //,  "bec"
      //,  "bef"
      //,  "beh"
      //,  "bel"
      //,  "ben"
      //,  "bes"
      //,  "bet"
      //,  "bey"
      //,  "ble"
      //,  "bl"
      //,  "brl"
      ,  "but"
      ,  "by"
      };
    case 'c':
      return std::list<std::string> {
         "c"
      ,  "can"
      //,  "cannot"
      ,  "cc"
      ,  "ch"
      ,  "character"
      //,  "child"
      //,  "chn"
      //,  "com"
      ,  "con"
      //,  "concv"
      //,  "concvg"
      //,  "cd"
      };
    case 'd':
      return std::list<std::string> {
         "d"
      ,  "day"
      ,  "dd"
      //,  "dcv"
      //,  "dcl"
      //,  "dclg"
      ,  "dis"
      ,  "do"
      };
    case 'e':
      return std::list<std::string> {
         "e"
      ,  "ea"
      ,  "ed"
      ,  "ei"
      ,  "en"
      ,  "ence"
      //,  "enough"
      ,  "er"
      ,  "ever"
      ,  "every"
      };
    case 'f':
      return std::list<std::string> {
         "f"
      ,  "father"
      ,  "ff"
      //,  "fst"
      ,  "for"
      //,  "fr"
      ,  "from"
      ,  "ful"
      };
    case 'g':
      return std::list<std::string> {
         "g"
      ,  "gg"
      ,  "gh"
      ,  "go"
      //,  "gd"
      //,  "grt"
      };
    case 'h':
      return std::list<std::string> {
         "h"
      ,  "had"
      ,  "have"
      ,  "here"
      //,  "herf"
      //,  "hm"
      //,  "hmf"
      //,  "his"
      };
    case 'i':
      return std::list<std::string> {
         "i"  
      //,  "imm"
      ,  "in"
      ,  "ing"
      //,  "into"
      ,  "it"
      ,  "ity"
      };
    case 'j':
      return std::list<std::string> {
         "j"
      ,  "just"
      };
    case 'k':
      return std::list<std::string> {
         "k"
      ,  "know"
      ,  "knowledge"
      };
    case 'l':
      return std::list<std::string> {
         "l"
      ,  "less"
      //,  "lr"
      ,  "like"
      //,  "ll"
      ,  "lord"
      };
    case 'm':
      return std::list<std::string> {
         "m"
      ,  "many"
      ,  "ment"
      ,  "more"
      ,  "mother"
      //,  "mch"
      //,  "mst"
      //,  "myf"
      };
    case 'n':
      return std::list<std::string> {
         "n"
      ,  "name"
      //,  "nec"
      //,  "nei"
      ,  "ness"
      ,  "not"
      };
    case 'o':
      return std::list<std::string> {
         "o"
      //,  "o\'c"
      ,  "of"
      ,  "one"
      //,  "onef"
      ,  "ong"
      ,  "ou"
      ,  "ought"
      ,  "ound"
      //,  "ourvs"
      ,  "ount"
      //,  "out"
      ,  "ow"
      };
    case 'p':
      return std::list<std::string> {
         "p"
      //,  "pd"
      ,  "part"
      ,  "people"
      //,  "percv"
      //,  "perh"
      };
    case 'q':
      return std::list<std::string> {
         "q"
      ,  "question"
      //,  "qk"
      ,  "quite"
      };
    case 'r':
      return std::list<std::string> {
         "r"
      ,  "rather"
      //,  "rcv"
      //,  "rcvg"
      //,  "rjc"
      //,  "rjcg"
      ,  "right"
      };
    case 's':
      return std::list<std::string> {
         "s" 
      //,  "sd"
      ,  "sh"
      //,  "shall"
      //,  "shd"
      ,  "sion"
      ,  "so"
      ,  "some"
      ,  "spirit"
      ,  "st"
      //,  "still"
      //,  "sch"
      };
    case 't':
      return std::list<std::string> {
         "t"
      ,  "th"
      ,  "that"
      ,  "the"
      ,  "their"
      //,  "themvs"
      ,  "there"
      ,  "these"
      //,  "this"
      ,  "those"
      ,  "through"
      ,  "time"
      ,  "tion"
      ,  "to"
      //,  "td"
      //,  "tgr"
      //,  "tm"
      //,  "tn"
      };
    case 'u':
      return std::list<std::string> {
         "u"
      //,  "under"
      //,  "upon"
      //,  "us"
      };
    case 'v':
      return std::list<std::string> {
         "v"
      //,  "very"
      };
    case 'w':
      return std::list<std::string> {
         "w"
      //,  "was"
      //,  "were"
      //,  "wh"
      //,  "where"
      //,  "which"
      ,  "whose"
      ,  "will"
      ,  "with"
      ,  "word"
      ,  "work"
      ,  "world"
      //,  "wd"
      };
    case 'x':
      return std::list<std::string> {
         "x"
      //,  "xs"
      //,  "xf"
      };
    case 'y':
      return std::list<std::string> {
         "y"
      ,  "you"
      ,  "young"
      //,  "yr"
      //,  "yrf"
      //,  "yrvs"
      };
    case 'z':
      return std::list<std::string> {
          "z"
      };
    default:    
      //return std::list<std::string> {""};
      std::cout << "possibles(): encountered unexpected character"
                << std::endl;
      exit(EXIT_FAILURE);
  }
}

//int main () {
    //std::list<std::string> list = possibles('a');
    //for (auto x : list)
        //std::cout << x << " ";
    //std::cout << std::endl;
//}
