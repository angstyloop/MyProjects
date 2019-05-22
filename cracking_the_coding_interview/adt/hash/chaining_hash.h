// an abstract base class for hashes that use chaining,
//  as well as my doubly linked list class.
#include "doubly_linked_list.cpp"

template<typename T>
class Hash {
    virtual unsigned h(const T&)=0;   // pure virtual hash function
    std::vector<std::shared_ptr<Node<T>>> table;  // vector of linked lists

public: 
    // ctr
    // initializes the vector of linked lists (table) to size_ nullptrs.
    Hash(unsigned size_): table (size_, nullptr) {}

    // dtr
    virtual ~Hash() {}

    void insert(const T&);
    std::unique_ptr<T> search(const T&);
    void remove(const T&);
    unsigned table_size() {return table.size();}
};

class SimpleStringHash : public Hash<std::string> {
    virtual unsigned h(const std::string&);
public:
    //ctr
    //just a wrapper for the base ctr
    SimpleStringHash(unsigned size_) : Hash<std::string>(size_) {}
    virtual ~SimpleStringHash() {}
};
