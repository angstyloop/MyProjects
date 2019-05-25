// Problem 2.4
// Check whether a string is a permutation of a palindrome.

// Key observation is that a palindrome has an even number
//  of each character.

#include<vector>
#include<string>
#include<iostream>

using namespace std;

// O(n) : use a count array. if odd entry return false, else true.
bool palindromePermutation(string& str) {
    vector<unsigned> arr(256, 0);     //count array
    for (char c : str)  //fill the array
        ++arr[int(c)];
    
    // if any char was repeated an odd number
    //  of times, str is not a palindrome.
    for (char c : str)
        if ( arr[int(c)] % 2 != 0 )
            return false;

    // otherwise str is a palindrome.
    return true;
}

int main() {
    string str = "bbccab";
    cout << palindromePermutation(str) << endl;
    return 0;
}


