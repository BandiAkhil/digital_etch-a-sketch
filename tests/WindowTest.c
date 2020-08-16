#include <stdio.h>
#include <assert.h>
#include "Window.h"

void testConstructor() {
    Window* window = Window__create(800, 400);
    assert(Window__getWidth(window) == 800);
    assert(Window__getHeight(window) == 400);
}

int main(int argc, char** argv) {
    testConstructor();
    printf("WindowTest Passed\n");
}
