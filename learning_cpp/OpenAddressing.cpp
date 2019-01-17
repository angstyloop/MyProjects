//open addressing with linear probing, quadratic probing, and double-hashing.
//quadratic probing is buggy: can't tell when hash table is actually full.
#include <iostream>
#define EMPTY 0
using namespace std;

class Hash {
    private:
        int h(int k, int i);
        int h1(int k);
        int h2(int k);
    public:
        int *table;
        int length;
        Hash(int N);
        ~Hash() { delete[] table; }
        void insert(int key);
        void print();
};

Hash::Hash(int N): length(N)
{
    table = new int[length];
    fill(table, table+length, EMPTY);
}

int Hash::h1(int k)
{
    return k % length;
}

int Hash::h2(int k)
{
    return 1 + k % (length-1);
}

int Hash::h(int k, int i)
{
    // linear probing
    return (h1(k) + i) % length;

    //quadratic probing;
    //return (h1(k) + 1*i + 3*i*i) % length;

    // double hashing
    //return (h1(k) + i*h2(k)) % length;
}

void Hash::insert(int k)
{
    int i = 0;
    if (table[h(k, i)] == EMPTY)
        table[h(k, i)] = k;
    else {
        // doesn't work for quadratic probing ...
        for (i = 1; table[h(k, i)] != EMPTY && h(k, i) != h(k, 0); ++i){}
        if (h(k, i) == h(k, 0))
            cout << "Hash table full." << endl;
        else table[h(k, i)] = k;
    }
}

void Hash::print()
{
    int* p = table;
    while (p < table+length) {
        if (*p == EMPTY)
            cout << "/";
        else cout << *p;
        cout << endl;
        ++p;
    }
}

int main()
{
    Hash H(11);
    int arr[9] {10, 22, 31, 4, 15, 28, 17, 88, 59};
    for (auto x: arr)
        H.insert(x);
    H.print();
}
