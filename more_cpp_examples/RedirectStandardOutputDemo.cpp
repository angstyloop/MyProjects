#include<iostream>
#include<fstream>

using std::ofstream;
using std::cout;
using std::endl;

class RedirectStandardOutputDemo {
  public:
    void test() { 
        ofstream os ("RedirectStandardOutputDemoOutputFile.txt", std::ios_base::trunc);
        auto cout_buf = cout.rdbuf(os.rdbuf());
        cout << "writes to the file now" << endl;
        cout.rdbuf(cout_buf);
         cout << "writes to standard output again." << endl;
    }
};

//int main() { RedirectStandardOutputDemo().test(); }
