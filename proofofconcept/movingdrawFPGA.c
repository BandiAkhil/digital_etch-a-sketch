#include <GL/glut.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <wiringPi.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include "MenuItem.h"
#include "MenuList.h"
#include "Line.h"
#include "Window.h"
#include "MenuBar.h"
#include "WindowManager.h"
#include "WindowPane.h"
#include "FpgaProtocol.h"
#include "Accelerometer.h"
#include "ShapePicker.h"
#include "FilePicker.h"
#include "Overlay.h"

bool reset = true;
bool menuMode = false;

int screenWidth;
int screenHeight;

int windowWidth;
int windowHeight;

Line* mainLine;
Window* mainWindow;
MenuBar* mainMenuBar;
WindowManager* mainWindowManager;
FpgaProtocol* mainProtocol;
Overlay* mainOverlay;

void onMovement(float x, float y, float t) {
    float prevT = Line__getThickness(mainLine);
    float newT = prevT + t;
    if (Line__setThickness(mainLine, newT)) {
        Overlay__setThickness(mainOverlay, newT);
    }
    if (!menuMode) {
        float prevX = Line__getPosition(mainLine)[0];
        float prevY = Line__getPosition(mainLine)[1];
        float aspect = (float)windowWidth / (float)windowHeight;
        float newX = prevX + x;
        float newY = prevY + y;
        Line__setPosition(mainLine, newX, newY);
    } else {
        if (y > 0) {
            MenuList* selectedList = MenuBar__getSelected(mainMenuBar);
            if (selectedList != NULL) {
                if (!WindowManager__isWindowOpen(mainWindowManager)) {
                    MenuList__selectPrevItem(selectedList);
                } else {
                    WindowManager__handleUp(mainWindowManager);
                }
            }
        } else if (y < 0) {
            MenuList* selectedList = MenuBar__getSelected(mainMenuBar);
            if (selectedList != NULL) {
                if (!WindowManager__isWindowOpen(mainWindowManager)) {
                    MenuList__selectNextItem(selectedList);
                } else {
                    WindowManager__handleDown(mainWindowManager);
                }
            }
        } else if (x > 0) {
            if (!WindowManager__isWindowOpen(mainWindowManager)) {
                MenuBar__selectNextList(mainMenuBar);
            } else {
                WindowManager__handleRight(mainWindowManager);
            }
        } else if (x < 0) {
            if (!WindowManager__isWindowOpen(mainWindowManager)) {
                MenuBar__selectPrevList(mainMenuBar);
            } else {
                WindowManager__handleLeft(mainWindowManager);
            }
        }
    }
}

void onColour(int r, int g, int b) {
    Line__setColour(mainLine, r, g, b);
    Overlay__setColour(mainOverlay, r, g, b);
}

void onReset(void* data) {
    reset = true;
}

void onButton(int* buttonArray) {
    if (buttonArray[0]) {
        if (menuMode) {
            if (!WindowManager__isWindowOpen(mainWindowManager)) {
                MenuList__callSelected(MenuBar__getSelected(mainMenuBar));
            } else {
                WindowManager__handleAction(mainWindowManager);
            }
        } else {
            char* filename = Window__captureToFile(mainWindow);
            char* message = (char*) malloc(sizeof(char) * 256);
            snprintf(message, 256, "Saved to %s", filename);
            Overlay__addMessage(mainOverlay, message);
        }
    } else if (buttonArray[1]) {
        menuMode = !menuMode;
        /*if (!menuMode) {
            printf("CLOSING MENU\n");
        } else {
            printf("OPENING MENU\n");
        }*/
    }
}

void openPane(void* _pane) {
    WindowPane* pane = (WindowPane*) _pane;
    WindowManager__openWindowPane(mainWindowManager, pane);
}

void saveScreen(void* v) {
    char* filename = Window__captureToFile(mainWindow);
    char* message = (char*) malloc(sizeof(char) * 256);
    snprintf(message, 256, "Saved to %s", filename);
    Overlay__addMessage(mainOverlay, message);
}

void transitionColour(void* v) {
    int tran = Line__toggleColourTransition(mainLine);
    if (tran) {
        Overlay__addMessage(mainOverlay, "Transition to next colour: On");
    } else {
        Overlay__addMessage(mainOverlay, "Transition to next colour: Off");
    }
}

bool isInit = false;

void init() {
    mainWindow = Window__create(windowWidth, windowHeight);

    mainProtocol = FpgaProtocol__create();
    FpgaProtocol__setMovementCallback(mainProtocol, onMovement);
    FpgaProtocol__setColourCallback(mainProtocol, onColour);
    FpgaProtocol__setButtonCallback(mainProtocol, onButton);
    Accelerometer* accelerometer = Accelerometer__create(onReset, NULL);

    mainWindow = Window__create(windowWidth, windowHeight);
    mainLine = Line__create(0.2f, (float)windowWidth / (float)windowHeight, 1.0f);

    mainWindowManager = WindowManager__create();

    WindowPane* loadPane = FilePicker__createPane();
    WindowPane* shapePane = ShapePicker__createPane(mainLine);

    MenuItem* saveFile = MenuItem__create("Save", saveScreen, NULL);
    MenuItem* loadFile = MenuItem__create("Load", openPane, loadPane);

    MenuItem* changeColour = MenuItem__create("Colour transition", transitionColour, NULL);
    MenuItem* changeShape = MenuItem__create("Change shape", openPane, shapePane);

    MenuItem** fileItems = (MenuItem**) malloc(sizeof(MenuItem*) * 2);
    fileItems[0] = saveFile;
    fileItems[1] = loadFile;

    MenuItem** editItems = (MenuItem**) malloc(sizeof(MenuItem*) * 2);
    editItems[0] = changeColour;
    editItems[1] = changeShape;

    MenuList* filesList = MenuList__create(fileItems, 2, "File");
    MenuList* editList = MenuList__create(editItems, 2, "Edit");

    MenuList** allLists = (MenuList**) malloc(sizeof(MenuList*) * 2);
    allLists[0] = filesList;
    allLists[1] = editList;

    mainMenuBar = MenuBar__create(allLists, 2);
    mainOverlay = Overlay__create();
}

bool pixelsLoaded = false;

void display() {
    if (!isInit) {
        init();
        isInit = true;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    if (reset) {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    reset = false;

    if (menuMode) {
        if (!pixelsLoaded) {
            Window__captureTexture(mainWindow);
            pixelsLoaded = true;
        }
        Window__drawTexture(mainWindow);
        MenuBar__draw(mainMenuBar, mainWindow);
        Overlay__draw(mainOverlay, mainWindow);
        WindowManager__draw(mainWindowManager, mainWindow);
    } else if (pixelsLoaded) {
        glClear(GL_COLOR_BUFFER_BIT);
        Window__drawTexture(mainWindow);
        pixelsLoaded = false;
    }

    if (!menuMode) {
        Line__draw(mainLine);
    }

    glFlush();
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    pid_t pid = getpid();
    setpriority(PRIO_PGRP, pid, -20);
    syscall(SYS_ioprio_set, 2, pid, 1);
    wiringPiSetup();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    if (argc > 1) {
        glutInitWindowSize(320, 320);
    } else {
        glutInitWindowSize(screenWidth, screenHeight);
    }
    glutCreateWindow("OpenGL Test");
    windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    //printf("w: %i, h: %i\n", windowWidth, windowHeight);
    glutInitWindowPosition(50, 50);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
