// A template version of ctci 1.1,
//  bool isUnique(vector<T>&);

#include<iostream>
#include<vector>
#define SETSIZE 1000 // size of hash table in Set class defined below

template <typename T>
class Node {
    Node* next;
    Node* prev;
    const T& data; 
public:
    Node(const T& data_) : data(data_) {}

    //~Node(){}

    const T& getData() const {return data;}

    Node* insert(Node* other) {
        other->prev = this->prev;
        other->next = this;
        this->prev->next = other;
        this->prev = other;
        return other;
    }

    Node* search(const T& c) {
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

template <typename T>
class Set {
    const unsigned size;
    std::vector<Node<T>*> table;
    unsigned hash(const T&) const;

public:
    Set(const unsigned size_) : size(size_), table(size_, nullptr) {}

    //~Set(){}

    const unsigned& getSize() const {return size;}
    
    bool insert(const T& c) {
        const unsigned i = hash(c);
        if (table[i] == nullptr) {
            table[i] = new Node<T>(c);
            return true; // data doesn't exist in our set yet, so insertion is successful!
        } else {
            Node<T>* match = table[i]->search(c);
            if (match==nullptr) {
                table[i]->insert(new Node<T>(c));
                return true; // data doesn't exist in our set yet, so insertion succeeds!
            } else return false; // data DOES exist already in the set, so insertion doesn't happen.
        }
    }
};

template <typename T>
bool isUnique(std::vector<T>& vec) {
    bool is_unique;
    Set<T> unique_chars (SETSIZE);
    for (T elem : vec ) {
        is_unique = unique_chars.insert(elem);
        if (is_unique) continue;
        else return false;
    }
    return true; // if we made it here, the set is unique.
}

// template specification of hash function for char type;
//  just use the ascii value modulo the size of the
//  hash table in set.
template<>
unsigned Set<char>::hash(const char& c) const {
    return int(c) % size;
}

int main() {
    std::vector<char> vec {'a','a','c'};
    std::cout << isUnique<char>(vec) << std::endl; 
}
