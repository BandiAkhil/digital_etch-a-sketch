#ifndef MENUBAR_H
#define MENUBAR_H
#include "MenuList.h"
#include "Style.h"

struct MenuBar;

typedef struct MenuBar MenuBar;

const static int maxLists = 10;

void MenuBar__init(MenuBar* self, MenuList** lists, int listCount);

MenuBar* MenuBar__create(MenuList** lists, int listCount);

void MenuBar__addList(MenuBar* self, MenuList* list);

void MenuBar__selectNextList(MenuBar* self);

void MenuBar__selectPrevList(MenuBar* self);

MenuList* MenuBar__getSelected(MenuBar* self);

void MenuBar__draw(MenuBar* self, Window* mainWindow);

#endif
