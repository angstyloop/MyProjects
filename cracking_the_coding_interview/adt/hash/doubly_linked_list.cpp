#include "doubly_linked_list.h"
#include <iostream>

// dtr
// unlinks the node when delete is called (unless there is only one node)
template<typename T>
Node<T>::~Node() {
    if (prev!=std::shared_ptr<Node>(this)) {
        if (prev!=nullptr) 
            prev->next = next;
        if (next!=nullptr) 
            next->prev = prev;
    }
}

// search for an element with a particular data value.
// assumes that objects of type T have the == operator defined.
template<typename T>
std::shared_ptr<Node<T>> Node<T>::search (T& data_) {
    const std::unique_ptr<Node<T>> start = this;    // pointer to head of list.
    std::unique_ptr<Node<T>> ptr = next;            // variable pointer to loop through list.
    if (*data_ptr==data_)       // if the head has the data we want, return the head pointer.
        return data_ptr;
    else while (ptr!=start) {   // otherwise loop through the list until the data is found
        if (*ptr==data_)        //   (and return it) or until we loop back to start.
            return ptr;
        else ptr = ptr->next;
    }
}

// unlink head and return pointer to new head.
template<typename T>
std::shared_ptr<Node<T>> Node<T>::pop_front() {
    const auto new_head = this->next; // save the new head.
    if (prev!=nullptr)          // unlink the old head.
        prev->next = next;
    if (next!=nullptr) 
        next->prev = prev;  
    return new_head;  // return the new head.
}

// insert an element at the front of the list.
template<typename T>
void Node<T>::push_front(const T& data_) {
    std::shared_ptr<Node<T>> node_ (new Node<T>(&data_)); // the new node wrapped in shared_ptr
    node_->next = std::shared_ptr<Node>(this);       // fix all the node pointers
    node_->prev = this->prev;  
    this->prev->next = node_;
    this->prev = node_;
}

//int main() {
    //using namespace std;
    //Node<string>* node_ptr = new Node<string>("hello");
    //cout << node_ptr->get_data() << std::endl;

//}
