#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "MenuItem.h"

MenuItem* testConstructor() {
    MenuItem* result = MenuItem__create("TestItem", NULL, "test");
    MenuItem__callOnClick(result);
    return result;
}

int onClickActivated = 0;

void onClick(void* _test) {
    char* test = (char*) _test;
    assert(strcmp(test, "test "));
    onClickActivated = 1;
}

void testOnClick(MenuItem* item) {
    MenuItem__setOnClick(item, onClick);
    MenuItem__callOnClick(item);
}

int main(int argc, char** argv) {
    MenuItem* item = testConstructor();
    testOnClick(item);
    printf("MenuItemTest Passed\n");
}
