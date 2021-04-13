#include <Arduino.h>

class Item {
  bool selected = false;
  void (*upDelegate)() = nullptr;
  void (*downDelegate)() = nullptr;
public:
  Item(void (*upDelegate)(), void (*downDelegate)()) {
    this->upDelegate = upDelegate;
    this->downDelegate = downDelegate;
  }

  bool isSelected() {
    return this->selected;
  }

  void select(bool selection) {
    this->selected = selection;
  }

  void up() {
    if(this->upDelegate != nullptr) {
      this->upDelegate();
    }
  }

  void down() {
    if(this->downDelegate != nullptr) {
      this->downDelegate();
    }
  }
};