#include "../../core/Card/Card.cpp"
void createCard(string deckName) {
    std::shared_ptr<Deck> d {new Deck{"/Users/sean/projects/CardApp/decks/" + deckName + ".deck"}};
    d.setNameFromInput();
    Card{d}.setNameFromInput()
          .setDir("/Users/sean/projects/CardApp/cards/example/")
          .setFrontFromInput()
          .setBackFromInput()
          .writeFrontToFile()
          .writeBackToFile();
    d->save();
}
int main(int argc, char** argv) { createCard(string{argv[1]}); }
