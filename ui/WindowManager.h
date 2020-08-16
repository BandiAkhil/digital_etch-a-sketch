#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#include "Window.h"
#include "WindowPane.h"

struct WindowManager;

typedef struct WindowManager WindowManager;

typedef struct WindowPane WindowPane;

void WindowManager__init(WindowManager* self);

WindowManager* WindowManager__create();

void WindowManager__handleUp(WindowManager* self);

void WindowManager__handleDown(WindowManager* self);

void WindowManager__handleLeft(WindowManager* self);

void WindowManager__handleRight(WindowManager* self);

void WindowManager__handleAction(WindowManager* self);

void WindowManager__openWindowPane(WindowManager* self, WindowPane* window);

void WindowManager__closeWindowPane(WindowManager* self);

int WindowManager__isWindowOpen(WindowManager* self);

void WindowManager__draw(WindowManager* self, Window* mainWindow);

#endif
