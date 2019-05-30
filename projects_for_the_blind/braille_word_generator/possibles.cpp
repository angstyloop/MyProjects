#include<list>
#include<string>
#include <iostream>

std::list<std::string> possibles (char first_letter) {
  switch(first_letter) {
    case 'a':
      return std::list<std::string> {
         "ab"
      ,  "abv"
      ,  "ac"
      ,  "acr"
      ,  "afn"
      ,  "afw"
      ,  "ag"
      ,  "agst"
      ,  "ally"
      ,  "alm"
      ,  "alr"
      ,  "al"
      ,  "alth"
      ,  "alt"
      ,  "alw"
      ,  "ance"
      ,  "and"
      ,  "ar"
      ,  "as"
      ,  "ation"
      };
    case 'b':
      return std::list<std::string> {
         "bb"
      ,  "be"
      ,  "bec"
      ,  "bef"
      ,  "beh"
      ,  "bel"
      ,  "ben"
      ,  "bes"
      ,  "bet"
      ,  "bey"
      ,  "ble"
      ,  "bl"
      ,  "brl"
      ,  "but"
      ,  "by"
      };
    case 'c':
      return std::list<std::string> {
         "can"
      ,  "cannot"
      ,  "cc"
      ,  "ch"
      ,  "character"
      ,  "child"
      ,  "chn"
      ,  "com"
      ,  "con"
      ,  "concv"
      ,  "concvg"
      ,  "cd"
      };
    case 'd':
      return std::list<std::string> {
         "day"
      ,  "dd"
      ,  "dcv"
      ,  "dcl"
      ,  "dclg"
      ,  "dis"
      ,  "do"
      };
    case 'e':
      return std::list<std::string> {
         "ea"
      ,  "ed"
      ,  "ei"
      ,  "en"
      ,  "ence"
      ,  "enough"
      ,  "er"
      ,  "ever"
      ,  "every"
      };
    case 'f':
      return std::list<std::string> {
         "father"
      ,  "ff"
      ,  "fst"
      ,  "for"
      ,  "fr"
      ,  "from"
      ,  "ful"
      };
    case 'g':
      return std::list<std::string> {
         "gg"
      ,  "gh"
      ,  "go"
      ,  "gd"
      ,  "grt"
      };
    case 'h':
      return std::list<std::string> {
         "had"
      ,  "have"
      ,  "here"
      ,  "herf"
      ,  "hm"
      ,  "hmf"
      ,  "his"
      };
    case 'i':
      return std::list<std::string> {
         "imm"
      ,  "in"
      ,  "ing"
      ,  "into"
      ,  "it"
      ,  "ity"
      };
    case 'x':
      return std::list<std::string> {
         "xs"
      ,  "xf"
      };
    case 'j':
      return std::list<std::string> {
         "just"
      };
    case 'k':
      return std::list<std::string> {
         "know"
      ,  "knowledge"
      };
    case 'l':
      return std::list<std::string> {
         "less"
      ,  "lr"
      ,  "like"
      ,  "ll"
      ,  "lord"
      };
    case 'm':
      return std::list<std::string> {
         "many"
      ,  "ment"
      ,  "more"
      ,  "mother"
      ,  "mch"
      ,  "mst"
      ,  "myf"
      };
    case 'n':
      return std::list<std::string> {
         "name"
      ,  "nec"
      ,  "nei"
      ,  "ness"
      ,  "not"
      };
    case 'o':
      return std::list<std::string> {
         "o\'c"
      ,  "of"
      ,  "one"
      ,  "onef"
      ,  "ong"
      ,  "ou"
      ,  "ought"
      ,  "ound"
      ,  "ourvs"
      ,  "ount"
      ,  "out"
      ,  "ow"
      };
    case 'p':
      return std::list<std::string> {
         "pd"
      ,  "part"
      ,  "people"
      ,  "percv"
      ,  "perh"
      };
    case 'q':
      return std::list<std::string> {
         "question"
      ,  "qk"
      ,  "quite"
      };
    case 'r':
      return std::list<std::string> {
         "rather"
      ,  "rcv"
      ,  "rcvg"
      ,  "rjc"
      ,  "rjcg"
      ,  "right"
      };
    case 's':
      return std::list<std::string> {
         "sd"
      ,  "sh"
      ,  "shall"
      ,  "shd"
      ,  "sion"
      ,  "so"
      ,  "some"
      ,  "spirit"
      ,  "st"
      ,  "still"
      ,  "sch"
      };
    case 't':
      return std::list<std::string> {
         "th"
      ,  "that"
      ,  "the"
      ,  "their"
      ,  "themvs"
      ,  "there"
      ,  "these"
      ,  "this"
      ,  "those"
      ,  "through"
      ,  "time"
      ,  "tion"
      ,  "to"
      ,  "td"
      ,  "tgr"
      ,  "tm"
      ,  "tn"
      };
    case 'u':
      return std::list<std::string> {
         "under"
      ,  "upon"
      ,  "us"
      };
    case 'v':
      return std::list<std::string> {
         "very"
      };
    case 'w':
      return std::list<std::string> {
         "was"
      ,  "were"
      ,  "wh"
      ,  "where"
      ,  "which"
      ,  "whose"
      ,  "will"
      ,  "with"
      ,  "word"
      ,  "work"
      ,  "world"
      ,  "wd"
      };
    case 'y':
      return std::list<std::string> {
         "you"
      ,  "young"
      ,  "yr"
      ,  "yrf"
      ,  "yrvs"
      };
    default:       //return empty list if input str doesnt match any contractions
      return std::list<std::string> {};
  }
}

int main () {
    std::list<std::string> list = possibles('y');
    for (auto x : list)
        std::cout << x << " ";
    std::cout << std::endl;
}
