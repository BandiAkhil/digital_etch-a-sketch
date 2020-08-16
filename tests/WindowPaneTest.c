#include <stdio.h>
#include <assert.h>
#include "WindowPane.h"
#include "WindowManager.h"

WindowPane* testConstructor() {
    WindowPane* result = WindowPane__create(2.0f, 4.0f, "TestPane");
    assert(WindowPane__getManager(result) == NULL);
    int* selected = WindowPane__getSelected(result);
    assert(selected[0] == 0);
    assert(selected[1] == -1);
    float* orthoLimits = WindowPane__getOrthoLimits(result);
    assert(orthoLimits[0] = -1.0f);
    assert(orthoLimits[1] = 1.0f);
    assert(orthoLimits[2] = -2.0f);
    assert(orthoLimits[3] = 2.0f);
    assert(WindowPane__isReset(result));
    return result;
}

void testReset(WindowPane* pane) {
    WindowPane__setSelected(pane, 0, 1);
    assert(!WindowPane__isReset(pane));
    WindowPane__reset(pane);
    assert(WindowPane__isReset(pane));
}

void testManager(WindowPane* pane) {
    WindowManager* testManager = WindowManager__create();
    WindowPane__setManager(pane, testManager);
    assert(WindowPane__getManager(pane) == testManager);
}

void testSelected(WindowPane* pane) {
    int* selected = WindowPane__getSelected(pane);
    WindowPane__onDown(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[1] == 0);
    WindowPane__onUp(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[1] == -1);
    WindowPane__onRight(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[0] == 1);
    WindowPane__onLeft(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[0] == 0);
}

void testLimits(WindowPane* pane) {
    int* selected = WindowPane__getSelected(pane);
    WindowPane__setXLimits(pane, 2, 2);
    selected = WindowPane__getSelected(pane);
    assert(selected[0] == 2);
    WindowPane__onLeft(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[0] == 2);
    WindowPane__onRight(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[0] == 2);

    WindowPane__setYLimits(pane, 4, 4);
    selected = WindowPane__getSelected(pane);
    assert(selected[1] == 4);
    WindowPane__onDown(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[1] == 4);
    WindowPane__onUp(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[1] == 3);
    WindowPane__onUp(pane);
    selected = WindowPane__getSelected(pane);
    assert(selected[1] == 3);
}

int main(int argc, char** argv) {
    WindowPane* pane = testConstructor();
    testReset(pane);
    testManager(pane);
    testSelected(pane);
    testLimits(pane);
    printf("WindowPaneTest Passed\n");
}
