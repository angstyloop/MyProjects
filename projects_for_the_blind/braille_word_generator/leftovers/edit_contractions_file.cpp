// Remove lines from braile_contractions.txt
//  that start with a '/'.

#include<string>
#include<iostream>
#include<fstream>

void edit_contractions_file () {
    const std::string filename =
        "./braille_contractions.txt";
    std::ifstream file (filename.c_str());
    std::string line;
    while (getline(file, line)) 
        if (line[0]!='/')
            std::cout << line + '\n';
}

int main() {
    edit_contractions_file();
    return 0;
}

