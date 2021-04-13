#include <Arduino.h>
#include <item.hpp>

class ItemsList {
  Item *items;
  uint8_t itemsCount = 0;
  uint8_t selectedItem = 0;
public:
  ItemsList(Item *items, uint8_t itemsCount) {
    this->items = items;
    this->itemsCount = itemsCount;
  }

  uint8_t getSelectedItem() {
    return this->selectedItem;
  }

  bool isItemSelected() {
    Serial.print("selected item = ");
    Serial.println(selectedItem);
    return items[selectedItem].isSelected();
  }

  bool isFirstItemSelected() {
    return selectedItem == 0;
  }

  bool isLastItemSelected() {
    return selectedItem == (itemsCount - 1);
  }

  void up() {
    if(items[selectedItem].isSelected()) {
      items[selectedItem].up();
      Serial.println("item modified up");
    } else {
      Serial.println("next item");
      if(selectedItem < (itemsCount - 1))
        selectedItem++;
    }
  }

  void down() {
    if(items[selectedItem].isSelected()) {
      items[selectedItem].down();
      Serial.println("item modified down");
    } else {
      Serial.println("prev item");
      if(selectedItem > 0)
        selectedItem--;
    }
  }

  void click() {
    Serial.println("selection changed");
    items[selectedItem].select(!items[selectedItem].isSelected());
  }
};