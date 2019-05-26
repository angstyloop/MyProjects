// CTCI Problem 1.6 : String Compression (i.e. run-length encoding)
// aaabbc -> a3b2c1  and etc.
// assume lowercase letters are the only characters.

#include <iostream>

using namespace std;

string stringCompression (string& s1) {
    const unsigned& L = s1.size();
    string s2 (L, '\0');
    unsigned i1, i2, run;
    s2[0] = s1[0];
    i1 = i2 = run = 1;

    while (i1 < L) {
        if (i2 >= L) 
            return s1; 
        else if (s1[i1-1] == s1[i1]) {
            ++run;
        }

        // if the character changes, write the run length and the new char
        else {
            s2[i2++] = char('0' + run); //write run length. easy way to convert digit to char.
            if (i2>=L)  // don't segfault pls
                return s1;
            s2[i2++] = s1[i1];  // new char
            run = 1; // reset the run length
        }
        ++i1;
    }
    
    return s2;
}

int main()
{
    string str = "aaaagghezzzzaabbcd";
    cout << stringCompression(str) << endl;
}
