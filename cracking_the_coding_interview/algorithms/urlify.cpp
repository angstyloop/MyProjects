// replace all spaces in a string with %20.
// can do it in place if we assume the input string has extra space.

#include <string>
#include <iostream>
#include <vector>

using namespace std;

string& urlify(string& str) {
    // count the number of spaces, get the true length of the string,
    //  and remember the index where the null character is stored.
    unsigned num_spaces  = 0
        ,    true_length = str.size()
        ,    used_length  = -1  // so we know if we never hit a null character
        ;

    int      i = 0
        ,    j = 0
        ;



    // count spaces up to the terminating null character
    while (str[i] != '\0') {
        if (str[i] == ' ') ++num_spaces; 
        ++i;
    }


    // save the length of the substring terminated by a null character
    used_length = i+1;

    // compute new length of string, and check
    //  that it's less than the length of the
    //  input string. It's just twice the
    //  number of spaces plus all the other chars.
    int new_length = 3*num_spaces + (used_length - num_spaces); 


    // makes sure the new length is smaller than the true length of the string
    assert(new_length <= true_length);

    
    // fill string from end, replacing spaces
    //  and leaving non-space characters alone.
    
    i = used_length-1;
    j = new_length-1;

    cout << "made it. i = " << i  << endl;
    cout << "made it. j = " << j  << endl;

    while (i > 0) {

        // if a space is encountered in the substring, copy the
        //  three characters "%20" backwards onto the open space
        //  at the new end of the string.
        cout << "made it. i = " << i  << endl;
        if (str[i] == ' ') {
            str[j]   = '0';
            str[j-1] = '2';
            str[j-2] = '%';
            j-=3;
        }

        // if not a space just copy and decrement j by 1
        else  {
            cout << "made it. i = " << i  << endl;
            cout << "made it. j = " << j  << endl;

            str[j] = str[i];
            --j;
        }

        // decrement i by 1 to get the next char in the old substring
        --i;
    }

    // the in-place string editing is finished. return ref to str.
    return str;
}

// test it out
int main() {
    string str = {'a', 'b', ' ', ' ', 'c', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    cout << urlify(str) << endl;
    return 0;
}
