// remove any characters that aren't a-z
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>

void az_filter(std::string& filename) {
    std::ifstream file (filename.c_str());
    std::string line;
    while (getline(file, line)) {
        for (char c : line)
            if (c >= 'a' && c <= 'z')
                std::cout << c;
        std::cout << std::endl;
    }
}

int main() {
    std::string str = "./braille_contractions.txt";
    az_filter(str);
    return 0;
}
