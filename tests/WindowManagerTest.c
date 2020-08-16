#include <assert.h>
#include <stdio.h>
#include "WindowManager.h"

WindowManager* testConstructor() {
    WindowManager* result = WindowManager__create();
    assert(!WindowManager__isWindowOpen(result));
    return result;
}

void testWindowOpen(WindowManager* manager) {
    WindowPane* testPane = WindowPane__create(200, 200, "test");
    WindowManager__openWindowPane(manager, testPane);
    assert(WindowManager__isWindowOpen(manager));
    WindowManager__closeWindowPane(manager);
    assert(!WindowManager__isWindowOpen(manager));
}

int main(int argc, char** argv) {
    WindowManager* manager = testConstructor();
    testWindowOpen(manager);
    printf("WindowManagerTest Passed\n");
}
