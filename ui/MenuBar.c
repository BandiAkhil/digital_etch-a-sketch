#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include "Window.h"
#include "MenuBar.h"
#include "MenuList.h"

struct MenuBar {
    MenuList** lists;
    int selectedList;
    /*private*/ int listCount;
};

typedef struct MenuBar MenuBar;

void MenuBar__init(MenuBar* self, MenuList** lists, int listCount) {
    self->lists = (MenuList**) malloc(sizeof(MenuList*) * 10);
    self->selectedList = -1;
    listCount = listCount > maxLists ? maxLists : listCount;
    if (listCount > 0) {
        memcpy(self->lists, lists, sizeof(MenuList*) * listCount);
    }
    self->listCount = listCount;
}

MenuBar* MenuBar__create(MenuList** lists, int listCount) {
    MenuBar* result = (MenuBar*) malloc(sizeof(MenuBar));
    MenuBar__init(result, lists, listCount);
    return result;
}

void MenuBar__addList(MenuBar* self, MenuList* list) {
    if (self->listCount < maxLists) {
        self->lists[self->listCount] = list;        
        self->listCount = self->listCount + 1;
    } else {
        printf("ListCount is already maxed, cannot add more lists.\n");
    }
}

void MenuBar__selectNextList(MenuBar* self) {
    if (self->selectedList < self->listCount - 1) {
        self->selectedList = self->selectedList + 1;
    }
}

void MenuBar__selectPrevList(MenuBar* self) {
    if (self->selectedList > -1) {
        self->selectedList = self->selectedList - 1;
    }
}

MenuList* MenuBar__getSelected(MenuBar* self) {
    if (self->selectedList > -1) {
        return self->lists[self->selectedList];
    } else {
        return NULL;
    }
}

void MenuBar__draw(MenuBar* self, Window* mainWindow) {
    float aspect = (float)Window__getWidth(mainWindow) / (float)Window__getHeight(mainWindow);
    //draw the background of the bar
    glColor3ub(mainColours[0], mainColours[1], mainColours[2]);
    glBegin(GL_QUADS);
        glVertex2f(-aspect, 1.0f - barHeight);
        glVertex2f(-aspect, 1.0f);
        glVertex2f(aspect, 1.0f);
        glVertex2f(aspect, 1.0f - barHeight);
    glEnd();
    glColor3ub(255, 255, 255);

    //draw the MenuLists
    for (int i = 0; i < self->listCount; i++) {
        MenuList__draw(self->lists[i], i, self->selectedList == i, mainWindow);
    }
}
