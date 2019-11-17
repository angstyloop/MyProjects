#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using std::vector;
using std::string;
using std::pair;
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::istreambuf_iterator;
using std::ostream_iterator;
using std::prev;
using std::cout;
using std::endl;

// stack of (id, path) pairs. to be used as a loose card container.
class Deck {
    string savePath = "/Users/sean/projects/CardApp/decks/example.deck";
    vector<string> paths;
    unsigned mapIdToIndex(unsigned id) { return id-1; }
    unsigned mapIndexToId(unsigned index) { return index+1; }
  public:
    Deck(){}
    // Each line of the deck's save file should be a path. This ctr parses them into the
    //  vector paths. Need to add format checking.
    Deck(string savePath): savePath(savePath) {
        ifstream ifs(savePath.c_str());
        string path;
        // should reserve here for performance
        while (ifs) {
            getline(ifs, path);
            ifs.ignore();
            push(path);
        }
    }
    Deck& clear() {
        paths.clear();
        return *this;
    }
    Deck& setNameFromInput() {
        setName(getInputString();)
        return *this;
    }
    unsigned getNextId() { return paths.size(); } //synonyms
    unsigned getCount() const { return paths.size(); } //synonyms
    Deck& push(string path) { 
        paths.push_back(path);
        return *this;
    }
    string pop() {
        const auto& path = paths.back();
        paths.pop_back();
        return path;
    }
    string getPathById(unsigned id) {
        return paths[mapIdToIndex(id)];
    }
    Deck& save() {
        ofstream ofs(savePath, std::ios::trunc);
        ostream_iterator<string> it (ofs, "\n");
        copy(paths.begin(), paths.end(), it);
        return *this;
    };
    Deck& test() {
        cout << "This is a deck test." << endl;
        // create a deck using the example deck file
        const string& deckSavePath {"/Users/sean/projects/CardApp/decks/example.deck"};
        Deck deck {deckSavePath};
        // clear old contents of example deck
        deck.clear();
        // add a card to the deck
        deck.push("/foo/bar/baz");
        // confirm deck size is 1
        assert(deck.getCount() == 1);
        // save the deck, overwriting the previous contents of the deck save file
        deck.save();
        // confirm the card path was saved to the deck save file
        ifstream ifs {deckSavePath.c_str()};
        string outputString{istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};
        assert(outputString == "/foo/bar/baz\n");
        return *this;
    }
};
