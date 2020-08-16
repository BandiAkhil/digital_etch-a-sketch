#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include "MenuList.h"
#include "MenuItem.h"
#include "MenuBar.h"
#include "Window.h"
#include "UIHelpers.h"

struct MenuList {
    MenuItem** items;
    char* title;
    int selectedItem;
    /*private*/ int itemCount;
};

typedef struct MenuList MenuList;

void MenuList__init(MenuList* self, MenuItem** items, int itemCount, char* title) {
    self->title = title;
    self->items = (MenuItem**) malloc(sizeof(MenuItem*) * maxItems);
    itemCount = itemCount > maxItems ? maxItems : itemCount;
    if (itemCount > 0) {
        memcpy(self->items, items, sizeof(MenuItem*) * itemCount);
    }
    self->selectedItem = 0;
    self->itemCount = itemCount;
}

MenuList* MenuList__create(MenuItem** items, int itemCount, char* title) {
    MenuList* result = (MenuList*) malloc(sizeof(MenuList));
    MenuList__init(result, items, itemCount, title);
    return result;
}

void MenuList__addItem(MenuList* self, MenuItem* item) {
    if (self->itemCount < maxItems) {
        self->items[self->itemCount] = item;        
        self->itemCount = self->itemCount + 1;
    } else {
        printf("ItemCount is already maxed, cannot add more items.\n");
    }
}

void MenuList__setTitle(MenuList* self, char* title) {
    self->title = title;
}

void MenuList__selectNextItem(MenuList* self) {
    if (self->selectedItem < self->itemCount - 1) {
        self->selectedItem = self->selectedItem + 1;
    }
}

void MenuList__selectPrevItem(MenuList* self) {
    if (self->selectedItem > 0) {
        self->selectedItem = self->selectedItem - 1;
    }
}

void MenuList__callSelected(MenuList* self) {
    if (self->itemCount > 0 && self->selectedItem < self->itemCount) {
        MenuItem__callOnClick(self->items[self->selectedItem]);
    }
}

void MenuList__draw(MenuList* self, int index, int selected, Window* mainWindow) {
    float aspect = (float)Window__getWidth(mainWindow) / (float)Window__getHeight(mainWindow);

    float x0 = listWidth * index;
    float x1 = listWidth * (index + 1);

    //draw highlight
    if (selected) {
        glColor3ub(mainColoursLight[0], mainColoursLight[1], mainColoursLight[2]);
        glBegin(GL_QUADS);
            glVertex2f(-aspect + x0, 1.0f - barHeight);
            glVertex2f(-aspect + x0, 1.0f);
            glVertex2f(-aspect + x1, 1.0f);
            glVertex2f(-aspect + x1, 1.0f - barHeight);
        glEnd();
    }

    //draw title
    int titleWidth = getBitmapWidth(self->title);
    int titleHeight = getBitmapHeight();
    float relativeTitleWidth = (float)titleWidth / (float)Window__getWidth(mainWindow) * aspect;
    float relativeTitleHeight = (float)titleHeight / (float)Window__getHeight(mainWindow);
    
    glColor3ub(0, 0, 0);
    drawBitmapText(self->title,
        (-aspect + x0) + (listWidth / 2) - (float)relativeTitleWidth,
        1.0f - barHeight / 2 - (float)relativeTitleHeight / 2);

    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2f(-aspect + x1, 1.0f - barHeight);
        glVertex2f(-aspect + x1, 1.0f);
    glEnd();
    glColor3ub(255, 255, 255);

    //draw MenuItems
    if (selected) {
        for (int i = 0; i < self->itemCount; i++) {
            MenuItem__draw(self->items[i], index, i, self->selectedItem == i, mainWindow);
        }
    }
};
