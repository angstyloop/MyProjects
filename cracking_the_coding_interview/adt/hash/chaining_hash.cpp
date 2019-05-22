#include "chaining_hash.h"
#ifndef IOSTREAM 
#include <iostream>

template <typename T>
void Hash<T>::insert(const T& elem) {
    const unsigned index = h(elem);             //generate index from elem using h().

    assert(index < table.size());               //check bounds before using []

    if (table[index]!=nullptr){

        std::cout << "collision!" << std::endl;
        table[index]->push_front(elem);
    }                  //if a list is there, add elem to it.

    else {
        std::cout << "no collision." << std::endl;
        std::cout << "index: " << index << std::endl;
        std::cout << "table size: " << table.size() << std::endl;
        table[index] = (std::shared_ptr<Node<T>>)(new Node<T>(elem));      //otherwise elem is the head.
    }
}

template <typename T>
void Hash<T>::remove(const T& elem) {
    const unsigned index = h(elem);
    if (table[index]!=nullptr) {
        while (table[index]->prev!=table[index]) // delete takes care of the pointers, so 
            if (table[index]->prev!=nullptr)     //     we can just keep deleting prev until
                delete table[index]->prev;       //     the list is empty.
        table[index] = nullptr;
    }
}

template <typename T>
std::unique_ptr<T> Hash<T>::search(const T& elem) {
    for (auto list : table)
        return list.search(elem)->data_ptr;
}

// a simple string hash function that sums the ascii 
//   character values and mods by the length of 
//   table (a vector of linked lists inherited
//   from the Hash class)
unsigned SimpleStringHash::h(const std::string& str) {
        unsigned sum = 0;
        for (char c : str)
            sum+= (unsigned) c;
        return  sum % table_size();
}

// driver program for testing 
int main() {
    const std::string str = "helloworld";
    const int table_size = 100;
    SimpleStringHash hash (table_size);
    hash.insert(str);
    //hash.insert(str);
    std::cout << "all done." << std::endl;
}

#endif

