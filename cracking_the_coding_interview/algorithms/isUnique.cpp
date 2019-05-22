// Problem 1.1
// determine if a string has all unique characters. if so, we say the string is "unique".
#include <string>
#include <vector>
#include<algorithm>
#include <iostream>
#define SETSIZE 1000

// O(N^2)  : brute force
bool isUnique_1(std::string str) {
    for (auto& c1: str)
        for (auto& c2: str)
            if (c1==c2 && &c1!=&c2) return 0;
    return true;
}

// O(N*logN) : uses sorting
bool isUnique_2(std::string str) {
    // O(N*logN) sort 
    std::sort(str.begin(), str.end());
    
    // now check if adjacent elements are equal in O(N) time
    for (int i=1; i<str.size(); i++)
        if (str[i]==str[i-1])
            return false;
    // if the loop finishes, the string is unique
    return true;
}

// O(N) : uses an boolean array of 256 bits, one for each character.
//  They all start off false, and each time a new character is encountered,
//  the corresponding bit is set to true. When an old character is encountered,
//  we return false. Otherwise, return true after the loop.

bool isUnique_3(std::string str) {
    // if the length of the string is larger than the size of the character set
    //  then at least one character must have been repeated.
    
    if (str.size() > 256) return false;
    else {
        bool unique_chars[256] {false};
        for (char c : str) {
            if (unique_chars[int(c)]) return false;
            else unique_chars[int(c)] = true;
        }
        // if the loop completes, str must be unique
        return true;
    }
}

// O(N), no outside data structures.
// same as above, just with a 32-bit unsigned integer instead of
//  a boolean array. this limits us to 32 characters, but that at least
//  gets us a-z. assume str is made up of a-z chars.
bool isUnique_4(std::string str) {
    if (str.size() > 26) return false; // str larger than the alphabet => something repeats
    else {
        unsigned unique_chars = 0
            ,    offset = 0
            ,    mask = 0
            ;
        for (char c : str) {
            offset = unsigned(c) - unsigned('a');
            mask = 1 << offset; //single bit mask
            if ( (unique_chars & mask) != 0 ) return false;
            else unique_chars = (unique_chars | mask);
        }
        return true; // the loop finished; str must be unique
    }
}

// O(N), using a Set, generalizeable 

// let's start with a simple Set class for characters. for this we'll need a doubly linked
//  list with char data.
class Node {

    Node* next {nullptr};
    Node* prev {nullptr};
    const char data;

public:
    //ctr
    Node(char data_) : data(data_) {}

    //dtr
    ~Node() {};

    // getter
    const char& getData() const {return data;}

    // insert a new node into the doubly linked list (at the front).
    //  return a pointer to the new head (the node you inserted).
    Node* insert(Node* other) {
        // pointer manipulations 
        other->prev = this->prev;
        other->next = this;
        this->prev->next = other;
        this->prev = other;
        
        // return head
        return other;
    }

    // search the DLL for a node with a particular data value
    //  and return a pointer to the node. if not found,
    //  return nullptr.
    Node* search(char c) {
        if (this->data == c) return this;
        else {
            // take advantage of the closed loop DLL.
            //  check each node for the data we want
            //  until we reach the node pointer we started
            //  with.
            Node* start = this; //cast
            Node* curr = this->next;
            while (curr != start)  {
                if (curr->data == c) return curr;
                else curr = curr->next;
            }
            // if we get here, none of the nodes in our DLL
            //  contain the correct data, so we return a 
            //  nullptr.
            return nullptr;
        }
    }
};

// now for the actual Set class. all we need is insert, provided that
//  insert returns true/false depending on whether that data already
//  exists in the Set. 
class Set {

    const unsigned size;
    std::vector<Node*> table; // hash table; vector of dll's

    // simple hash function: ascii character value mod size of the hash table
    unsigned hash(char c) {
        return int(c) % size; 
    }

public:
    // ctr
    Set(unsigned size_) 
        : size(size_)
        , table(size_, nullptr) 
    {}

    // dtr
    ~Set() {}

    // getter
    const unsigned& getSize() const {return size;}

    //insert a new datum into the hash table and return true, unless it already exists,
    //  in which case return false.
    bool insert(char c) {
        const unsigned i = hash(c); 
        if (table[i] == nullptr) {
            table[i] = new Node(c);
            return true; // data doesn't exist in our set yet, so insertion is successful!
        } else {
            Node* match = table[i]->search(c);
            if (match==nullptr) {
                table[i]->insert(new Node(c));
                return true; // data doesn't exist in our set yet, so insertion succeeds!
            } else return false; // data DOES exist already in the set, so insertion doesn't happen.
        }
    }
};

// finally we can use the Set class defined above to write isUnique.
bool isUnique_5(std::string str) {
    bool is_unique;
    Set unique_chars(SETSIZE);
    for (int i=0; i<str.size(); ++i) {
        is_unique = unique_chars.insert(str[i]);
        if (is_unique) continue;
        else return false;
    }
    return true; // if we made it here, the string is unique.
}

int main() {
    std::cout << isUnique_5("abcdeff") <<std::endl;
}
