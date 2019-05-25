// given three operations:
// 1) delete a character
// 2) change a character
// 3) append a character
// write a function (pair<string>->bool) that returns true if the 
// two string arguments are 1 edit away from each other, and false
// otherwise. If the strings are equal, we would like to return true.

// O(n) : keep two indices, and go through each string simultaneously,
// keeping track of character differences and the number of edits.

// delete: ++index of the longer string
// change: ++ both indices
// append: ++ both indices: 

#include <cstdlib>
#include <iostream>

using namespace std;

bool oneAway(string& s1, string& s2) {
    const int&      l1 = s1.size()
                ,   l2 = s2.size()
                ;

    if (abs(l1-l2)>1) return false; // if the lengths differ by 2 or more
                                    //  then the strings can't be 1 edit away
    unsigned  i1 = 0
            , i2 = 0
            , edits = 0 // if this ever exceeds 1 we return false
            ;

    while (i1<l1 && i2<l2) {
        if (s1[i1] != s2[i2]) {
            if (edits==1)       // if we already made an edit, we can't make any more
                return false;
            else if (l1>l2)     // if one string is bigger, we want to delete from that char.
                ++i1;           //   note appending has same effect on i1, i2 as deleting.
            else if (l2>l1)
                ++i2;
            else {              // otherwise we advance both indices
                ++i1;
                ++i2;
            }
            ++edits;
        }
        else {
            ++i1;
            ++i2;
        }
    }

    return true; // if we make it here the strings are one edit away from each other
}

//test it out
int main() {
    string  s1 = "abcd"
        ,   s2 = "acd" 
        ;

    cout<<oneAway(s1, s2)<<endl;

    return 0;
}
