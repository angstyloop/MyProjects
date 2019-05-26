// Hashing by chaining
#include <iostream>
#include <math.h>
using namespace std;

// Linked list node class
class LLNode {
    public:
        int key;
        LLNode* next;
        LLNode(int arg, LLNode* ptr = nullptr): key(arg), next(ptr) {} 
};

// Hash class
class Hash {
    private:
        //division hashing
        //int h(int k) {return k%length;}
        //multiplication hashing
        int h(int k) { return int(9*(k*sqrt(2) - int(k*sqrt(2)))); }
    public:
        int length;     // length of the hash table
        LLNode** table; // array of pointers to linked list nodes
        Hash(int N);
        ~Hash() {delete[] table;} // explicit destructor
        void insert(int key);
        void print();   // prints the array of linked lists
};

// constructor creates a hash table of length N and fills it with nullptr
Hash::Hash(int N): length(N)
{
    table = new LLNode*[length];
    fill(table, table+length, nullptr);
}

// insert a key into the hash table
void Hash::insert(int key)
{
    int i = h(key);
    table[i] = new LLNode(key, table[i]); // expects table to be initialized with nullptrs
} 

void Hash::print()
{
    LLNode** p = table;
    LLNode* q;
    while (p < table+length) {
        q = *p++;
        if (q) {
            cout << q->key << " ";
            while (q->next != nullptr) {
                cout << q->next->key << " ";
                q = q->next;    // note ++q won't work here bc it's not an array 
            }
            cout << endl;
        }
        else cout << "/" << endl;
    } 
}

int main()
{
    Hash H(9);
    int arr[9] {5, 28, 19, 15, 20, 33, 12, 17, 10};
    for (auto x: arr)
        H.insert(x);
    H.print();
}
