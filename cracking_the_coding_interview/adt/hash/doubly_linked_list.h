// a template class for doubly linked lists
#include <vector>
#include <string>

template<typename T>
class Node {
  std::unique_ptr<T> data_ptr = nullptr;
  std::shared_ptr<Node> prev = nullptr
                        , next = nullptr
                        ;
public:
  //ctr
  Node() {}
  Node(T* data_ptr_): data_ptr(data_ptr_) {}

  //dtr
  ~Node();

  std::shared_ptr<Node<T>> search(T&);      // search for the node with matching data
  std::shared_ptr<Node<T>> pop_front();     // remove and return the head of the linked list
  void push_front(const T&);    // add a new element to the linked list
  const T& get_data() {return *data_ptr;}; 
};




