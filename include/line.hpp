#include <Arduino.h>
#include <items_list.hpp>

class Line {
  ItemsList *list = nullptr;
  void (*printDelegate)() = nullptr;
  void (*clickDelegate)() = nullptr;
public:
  Line(void (*printDelegate)()) {
    this->printDelegate = printDelegate;
  }

  Line(void (*printDelegate)(), void (*clickDelegate)()) : Line(printDelegate) {
    this->clickDelegate = clickDelegate;
  }

  Line(ItemsList * list, void (*printDelegate)()) : Line(printDelegate) {
    this->list = list;
  }

  void update() {
    if(printDelegate != nullptr) {
      printDelegate();
    }
  }

  bool up() {
    bool canChangeLine = true;
    if(list != nullptr) {
      if(list->isItemSelected()) canChangeLine = false;
      Serial.println("item is not modifying!");
      if(!list->isLastItemSelected()) canChangeLine = false;
      Serial.println("Last item selected!");
      
      list->up();
    }
    return canChangeLine;
  }

  bool down() {
    bool canChangeLine = true;
    if(list != nullptr) {
      if(list->isItemSelected()) canChangeLine = false;
      Serial.println("Item is not modifying!");
      if(!list->isFirstItemSelected()) canChangeLine = false;
      Serial.println("First item selected!");
      
      list->down();
    }
    return canChangeLine;
  }

  void click() {
    if(clickDelegate != nullptr) {
      clickDelegate();
    }

    if(list != nullptr) {
      list->click();
    }
  }
};