#include <stdlib.h>
#include <stdio.h>
#include "MenuList.h"
#include "MenuItem.h"

MenuList* testConstructor() {
    MenuList* result = MenuList__create(NULL, 0, "TestList");
    MenuList__callSelected(result);
    return result;
}

void testMaxItems(MenuList* list, MenuItem* testItem) {
    for (int i = 0; i < maxItems; i++) {
        MenuList__addItem(list, testItem);
    }
}

int main(int argc, char** argv) {
    MenuItem* testItem = MenuItem__create("TestItem", NULL, NULL);
    MenuList* list = testConstructor();
    testMaxItems(list, testItem);
    printf("MenuListTest Passed\n");
}
