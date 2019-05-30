// 
// Use braille_contractions.txt to generate the switch statement in
//  possibles.cpp.
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void create_switch_statement () {
    std::string   possibles_filename      ="./possibles.cpp" 
              ,   contractions_filename   ="./braille_contractions.txt"
              ,   header = 
// begin header
"#include<list>\n\
#include<string>\n\
\n\
std::list<std::string> possibles (char first_letter) {\n\
  switch(first_letter) {\n"
// end header
              ,   footer = 
// begin footer
"  }\n\
}\n";
// end footer

    std::ifstream   infile  (contractions_filename.c_str())
                ;

    std::string     contraction
                ,   case_head
                ,   case_footer
                ;

    bool changed = false;
    char first_letter = '\0'; // Only a-z allowed, so this is a
                             // safe unique choice for an initial value;.
    std::cout << header;

    while (getline(infile, contraction)) {
        changed = contraction[0]!=first_letter;
        if (changed) {
            
            case_footer =           //using old first_letter

//begin case footer
"      };\n";
//end old case footer. there will be an extra one at the beginning for '\0'.
            std::cout << case_footer;

            first_letter = contraction[0];

            case_head =             //using new first_letter

//begin new case head
std::string("    case \'") + first_letter + std::string("\':\n\
      return std::list<std::string> {\n         \"") + contraction + "\"\n";
//end case head

            std::cout << case_head;

        } else {
            
            first_letter = contraction[0];
            std::cout << "      ,  \"" + contraction + "\"\n";
        }
    } 
    std::cout << footer << std::endl;
}

int main() {
    create_switch_statement();     
}
