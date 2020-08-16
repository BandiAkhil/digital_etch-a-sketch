#include <stdlib.h>
#include "Window.h"
#include "WindowManager.h"

struct WindowManager {
    WindowPane* currentPane;
};

typedef struct WindowManager WindowManager;

void WindowManager__init(WindowManager* self) {
    self->currentPane = NULL;
}

WindowManager* WindowManager__create() {
    WindowManager* result = (WindowManager*) malloc(sizeof(WindowManager));
    WindowManager__init(result);
    return result;
}

void WindowManager__handleUp(WindowManager* self) {
    if (self->currentPane != NULL) {
        WindowPane__onUp(self->currentPane);
    }
}

void WindowManager__handleDown(WindowManager* self) {
    if (self->currentPane != NULL) {
        WindowPane__onDown(self->currentPane);
    }
}

void WindowManager__handleLeft(WindowManager* self) {
    if (self->currentPane != NULL) {
        WindowPane__onLeft(self->currentPane);
    }
}

void WindowManager__handleRight(WindowManager* self) {
    if (self->currentPane != NULL) {
        WindowPane__onRight(self->currentPane);
    }
}

void WindowManager__handleAction(WindowManager* self) {
    if (self->currentPane != NULL) {
        WindowPane__handleAction(self->currentPane);
    }
}

void WindowManager__openWindowPane(WindowManager* self, WindowPane* window) {
    WindowPane__setManager(window, self);
    self->currentPane = window;
}

void WindowManager__closeWindowPane(WindowManager* self) {
    WindowPane__reset(self->currentPane);
    self->currentPane = NULL;
}

int WindowManager__isWindowOpen(WindowManager* self) {
    return self->currentPane != NULL;
}

void WindowManager__draw(WindowManager* self, Window* mainWindow) {
    if (self->currentPane != NULL) {
        WindowPane__draw(self->currentPane, mainWindow);
    }
}
