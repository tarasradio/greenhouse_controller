#include <Arduino.h>
#include <page.hpp>

class Menu {
  Page * pages;
  uint8_t selectedPage = 0;
public:
  Menu(Page *pages) {
    this->pages = pages;
  }

  uint8_t getSelectedPage() {
    return this->selectedPage;
  }

  void selectPage(uint8_t page) {
    this->selectedPage = page;
  }

  void selectParentPage() {
    uint8_t page = this->pages[selectedPage].getParentPage();
    if(page != -1) {
      this->selectedPage = page;
    }
  }

  uint8_t getSelectedLine() {
    return this->pages[this->selectedPage].getSelectedLine();
  }

  uint8_t getSelectedPageLinesCount() {
    return this->pages[this->selectedPage].getLinesCount();
  }

  void update() {
    pages[selectedPage].update();
  }
  void up() {
    pages[selectedPage].up();
  }
  void down() {
    pages[selectedPage].down();
  }
  void click() {
    pages[selectedPage].click();
  }
};