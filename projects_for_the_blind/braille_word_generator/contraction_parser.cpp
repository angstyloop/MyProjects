// contraction_parser
// a helpful parsing function for extracting
// braille contractions from the text file I copied
// off the internet (http://www.brl.org)

// just pipe the output to a file -> did this!
//  the file is called parsed_braille_contractions.txt

#include <iostream>
#include <fstream>
#include <regex>

void contraction_parser (std::string& filename) {
    std::ifstream file (filename.c_str());//open file
    assert(file.good());    //check file exists
    std::pair<std::string,std::string> p;
    std::string contraction;
    while (file >> p.first >> p.second){//read line by line
        // if the 2nd arg is a number, the first
        //  arg is a contraction.
        if (isdigit(p.second[0])) {
            contraction = p.first; 
        } else {
        // otherwise the 2nd arg is the contraction.
            contraction = p.second; 
        }
        //print contraction to std output
        std::cout << contraction << std::endl;
    }
}

int main() {
    std::string filename = "./braille_contractions.txt";
    contraction_parser(filename);
    return 0;
}
