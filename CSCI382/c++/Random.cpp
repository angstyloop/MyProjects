/* Some practice with generating random numbers in c++ */
/* I added a function Shuffle() that randomizes a list by swapping each element an element at a random index. */

#include <iostream>
#include <random>
using namespace std;

// we'll need a Swap() function
template <class T>
void Swap(vector<T> &A, int i, int j)
{
    T c = A[i];
    A[i] = A[j];
    A[j] = c;
}

template <class T>
void Shuffle(vector<T> &A)
{
    // generate a distribution of random indices
    random_device seed;
    mt19937 generator(seed());
    uniform_int_distribution<int> distribution(0,A.size()-1);
    
    // swap every element in the list with a randomly selected element
    for (int i=0; i<A.size(); i++)
    {
        Swap(A, i, distribution(generator));
    }
}

// Shuffle some arguments around from the command line
int main(int argc, char **argv)
{
    // Get the arguments from the command line into an array of strings.
    vector<string> A(argv+1, argv+argc);
    // Shuffle and print the shuffled arguments.
    Shuffle(A);
    for (auto x: A) cout << x << " ";
    cout << endl;
    return 0;
}

// This is just an example I found online for how to generate random numbers in c++.
// There are three components: the seed, the generator, and the distribution.
// The generator takes a seed, and the distribution takes the generator.
/*
int main()
{
    //Random seed
    random_device rd;

    //Initialize Mersenne Twister pseudo-random number generator
    mt19937 gen(rd());
    
    // Generate pseudo-random numbers
    //  uniformly distributed in range (1,100)
    uniform_int_distribution<int> dis(1,2);

    for (int i=0; i<10; i++)
    {
        //int randomX = dis(gen);
        cout << dis(gen)  << endl;
    }
}
*/
