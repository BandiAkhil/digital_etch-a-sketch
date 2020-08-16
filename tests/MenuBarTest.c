#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MenuBar.h"
#include "MenuList.h"

MenuBar* testConstructor(MenuList* testList) {
    MenuList** lists = (MenuList**) malloc(sizeof(MenuList*));
    lists[0] = testList;
    MenuBar* result = MenuBar__create(lists, 1);
    assert(MenuBar__getSelected(result) == NULL);
    return result;
}

void testListMax(MenuBar* menu, MenuList* testList) {
    for (int i = 1; i < maxLists + 1; i++) {
        MenuBar__addList(menu, testList);
    }
}

void testSelect(MenuBar* menu, MenuList* testList) {
    MenuBar__selectNextList(menu);
    assert(MenuBar__getSelected(menu) == testList);
    MenuBar__selectPrevList(menu);
    assert(MenuBar__getSelected(menu) == NULL);
}

int main(int argc, char** argv) {
    MenuList* testList = MenuList__create(NULL, 0, "TestList");
    MenuBar* menu = testConstructor(testList);
    testSelect(menu, testList);
    testListMax(menu, testList);
    printf("MenuBarTest Passed\n");
}
