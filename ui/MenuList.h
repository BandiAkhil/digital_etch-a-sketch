#ifndef MENULIST_H
#define MENULIST_H
#include "MenuItem.h"
#include "Window.h"

struct MenuList;

typedef struct MenuList MenuList;

const static int maxItems = 20;

void MenuList__init(MenuList* self, MenuItem** items, int itemCount, char* title);

MenuList* MenuList__create(MenuItem** items, int itemCount, char* title);

void MenuList__addItem(MenuList* self, MenuItem* item);

void MenuList__setTitle(MenuList* self, char* title);

void MenuList__selectNextItem(MenuList* self);

void MenuList__selectPrevItem(MenuList* self);

void MenuList__callSelected(MenuList* self);

void MenuList__draw(MenuList* self, int index, int selected, Window* mainWindow);

#endif
