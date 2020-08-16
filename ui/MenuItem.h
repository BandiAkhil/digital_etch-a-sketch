#ifndef MENUITEM_H
#define MENUITEM_H
#include "Window.h"

struct MenuItem;

typedef struct MenuItem MenuItem;

void MenuItem__init(MenuItem* self, char* title, void (*onClick)(void*), void* userData);

MenuItem* MenuItem__create(char* title, void (*onClick)(void*), void* userData);

void MenuItem__setOnClick(MenuItem* self, void (*onClick)(void*));

void MenuItem__callOnClick(MenuItem* self);

void MenuItem__setTitle(MenuItem* self, char* title);

void MenuItem__draw(MenuItem* self, int listIndex, int index, int selected, Window* mainWindow);

#endif
