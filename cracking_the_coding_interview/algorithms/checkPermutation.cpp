// ctci problem 1.2
// write a program that determines whether or not two strings
//  are permutations of each other.
#include<string>
#include<iostream>
#include<vector>

// O(N^2): Brute-force solution 
bool checkPermutation_1(std::string& s1, std::string& s2) { 
    if (s1.size()!=s2.size()) return false;
    bool found;
    for (char c1: s1) {
        // check if c1 is in s2
        found = false;
        for (char c2: s2) {
            if (c2==c1){
                found = true;
                break;
            } 
        }
        if (!found) return false;
        //else continue;
    }
    // if we made it here, then the strings are the same size
    //  and every character in s1 is also in s2.
    return true;
}

// O(N): build two frequency tables and compare them
bool checkPermutation_2 (const std::string& s1, const std::string& s2) {
    if (s1.size() != s2.size()) return false;

    //else...
    std::vector<unsigned> freq1 (256, 0)
                        , freq2 (256, 0)
                        ; 
    
    for (char c : s1) ++freq1[int(c)];
    for (char c : s2) ++freq2[int(c)];

    //look for differences...
    for (int i=0; i<256; ++i)
        if (freq1[i] != freq2[i])
            return false;

    //if none are found then the two strings are permutations
    return true;
}

int main() {
    using namespace std;
    string  s1 = "hello";
    string  s2 = "olleh";
    std::cout << checkPermutation_1(s1, s2) << std::endl;
    return 0;
}
