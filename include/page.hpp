#include <Arduino.h>
#include <line.hpp>

class Page {
  Line *lines;
  uint8_t linesCount = 0;
  uint8_t selectedLine = 0;

  uint8_t parentPage = 0;

  void (*clickDelegate)() = nullptr;
public:
  Page(Line *lines, uint8_t linesCount, uint8_t parentPage) {
    this->lines = lines;
    this->linesCount = linesCount;
    this->parentPage = parentPage;
  }

  Page(Line *lines, uint8_t linesCount, void (*clickDelegate)(), uint8_t parentPage) : Page(lines, linesCount, parentPage) {
    this->clickDelegate = clickDelegate;
  }

  uint8_t getParentPage() {
    return this->parentPage;
  }

  uint8_t getSelectedLine() {
    return this->selectedLine;
  }

  uint8_t getLinesCount() {
    return this->linesCount;
  }

  void update() { //TODO: переделать
    for(uint8_t i = 0; i < linesCount; i++) {
      lines[i].update();
    }
  }
  
  void up() {
    Line *line = &lines[selectedLine];

    bool canChangeLine = line->up();
    bool isNotLastLine = selectedLine < (linesCount - 1);

    if(canChangeLine && isNotLastLine) {
        selectedLine++;
    }
  }

  void down() {
    Line *line = &lines[selectedLine];
    
    bool canChangeLine = line->down();

    if(canChangeLine && (selectedLine > 0)) {
        selectedLine--;
    }
  }

  void click() {
    if(clickDelegate != nullptr) {
      clickDelegate();
    } else {
      lines[selectedLine].click();
    }
  }
};