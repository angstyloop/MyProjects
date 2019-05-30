// go through each line of input file, writing only the lines that start with a-z to std out

#include<iostream>
#include<string>
#include<fstream>


using s_t = std::string;

void az_filter (s_t& fname) {
    bool lowercase;
    std::ifstream infile (fname.c_str());
    s_t line;
    
    // go through each line, and only print it if every character in the line is a-z
    while (getline(infile, line)) {
        lowercase = true;
        for (int i=0; i<line.size()-1; i++)
            if (line[i] < 'a' || line[i] > 'z'){
                lowercase = false;
                break;
            }
        if (lowercase)
            std::cout << line << '\n';
    }
}

int main() {
    s_t fname = "./words.txt";
    az_filter(fname);
    return 0;
}

