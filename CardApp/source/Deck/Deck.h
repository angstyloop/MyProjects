#include <iostream>
#include <vector>
#include <fstream>
using std::vector;
using std::string;
using std::pair;
using std::ofstream;
using std::ostream_iterator;
using std::prev;
using std::cout;
using std::endl;

// stack of (id, path) pairs. to be used as a loose card container.
class Deck {
    string savePath = "/Users/sean/projects/CardApp/decks/temp.deck";
    vector<string> paths;
    unsigned mapIdToIndex(unsigned id) { return id-1; }
    unsigned mapIndexToId(unsigned index) { return index+1; }
  public:
    Deck(){}
    Deck(string savePath): savePath(savePath) {}
    unsigned getNextId() { return paths.size()+1; }
    unsigned getCount() const { return paths.size(); }
    Deck& pushPath(string path) { 
        paths.push_back(path);
        return *this;
    }
    string popPath() {
        const auto& path = paths.back();
        paths.pop_back();
        return path;
    }
    string getPathById(unsigned id) {
        return paths[mapIdToIndex(id)];
    }
    Deck& save() {
        ofstream ofs(savePath);
        ostream_iterator<string> it (ofs, "\n");
        copy(paths.begin(), prev(paths.end()), it);
        ofs << paths.back();
        return *this;
    };
    Deck& test() {
        cout << "This is a deck test." << endl;
        Deck deck {"/Users/sean/projects/CardApp/decks/example.deck"};
        return *this;
    }
};
