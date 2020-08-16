#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "MenuItem.h"
#include "MenuList.h"
#include "Line.h"
#include "Window.h"
#include "MenuBar.h"
#include "WindowManager.h"
#include "WindowPane.h"
#include "FilePicker.h"
#include "ShapePicker.h"
#include "Overlay.h"

float moveSpeed = 0.0003f;

bool heldArrows[4];

float offsetsUpRight[] = { 0.0f, 0.0f };
float squareSize[] = { 0.01f, 0.01f };
float circleSize = 0.01f;
float sizeIncrease = 0.001f;

int colors[] = {0,0,0};

bool reset = true;
bool menuMode = false;

int screenWidth;
int screenHeight;

int windowWidth;
int windowHeight;

Line* mainLine;
Window* mainWindow;
MenuBar* mainMenuBar;
Overlay* mainOverlay;
WindowManager* mainWindowManager;

void onLeft() {
    float lineX = Line__getPosition(mainLine)[0];
    float lineY = Line__getPosition(mainLine)[1];
    float aspect = (float)windowWidth / (float)windowHeight;
    if (Line__getPosition(mainLine)[0] > -aspect) {
        Line__setPosition(mainLine, lineX - moveSpeed, lineY);
    }
}

void onRight() {
    float lineX = Line__getPosition(mainLine)[0];
    float lineY = Line__getPosition(mainLine)[1];
    float aspect = (float)windowWidth / (float)windowHeight;
    if (Line__getPosition(mainLine)[0] < aspect) {
        Line__setPosition(mainLine, lineX + moveSpeed, lineY);
    }
}

void onUp() {
    float lineX = Line__getPosition(mainLine)[0];
    float lineY = Line__getPosition(mainLine)[1];
    float aspect = (float)windowWidth / (float)windowHeight;
    if (Line__getPosition(mainLine)[1] < aspect) {
        Line__setPosition(mainLine, lineX, lineY + moveSpeed);
    }
}

void onDown() {
    float lineX = Line__getPosition(mainLine)[0];
    float lineY = Line__getPosition(mainLine)[1];
    float aspect = (float)windowWidth / (float)windowHeight;
    if (Line__getPosition(mainLine)[1] > -aspect) {
        Line__setPosition(mainLine, lineX, lineY - moveSpeed);
    }
}

void upSize() {
    float lineT = Line__getThickness(mainLine);
    if (lineT < 10.0f) {
        Line__setThickness(mainLine, lineT + sizeIncrease);
        Overlay__setThickness(mainOverlay, lineT + sizeIncrease);
    }
}

void downSize() {
    float lineT = Line__getThickness(mainLine);
    if (lineT > 0.0f) {
        Line__setThickness(mainLine, lineT - sizeIncrease);
        Overlay__setThickness(mainOverlay, lineT - sizeIncrease);
    }
}

bool callHeldInputs() {
    bool hasChanged = false;
    if (!menuMode) {
        if (heldArrows[0]) {
            hasChanged = true;
            onUp();
        }
        if (heldArrows[1]) {
            hasChanged = true;
            onDown();
        }
        if (heldArrows[2]) {
            hasChanged = true;
            onLeft();
        }
        if (heldArrows[3]) {
            hasChanged = true;
            onRight();
        }
    }
    return hasChanged;
}

void resetScreen() {
    reset = true;
}

void readSpecialKeyboard(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            if (menuMode) {
                MenuList* selectedList = MenuBar__getSelected(mainMenuBar);
                if (selectedList != NULL) {
                    if (!WindowManager__isWindowOpen(mainWindowManager)) {
                        MenuList__selectPrevItem(selectedList);
                    } else {
                        WindowManager__handleUp(mainWindowManager);
                    }
                }
            } else {
                heldArrows[0] = true;
            }
            break;
        case GLUT_KEY_DOWN:
            if (menuMode) {
                MenuList* selectedList = MenuBar__getSelected(mainMenuBar);
                if (selectedList != NULL) {
                    if (!WindowManager__isWindowOpen(mainWindowManager)) {
                        MenuList__selectNextItem(selectedList);
                    } else {
                        WindowManager__handleDown(mainWindowManager);
                    }
                }
            } else {
                heldArrows[1] = true;
            }
            break;
        case GLUT_KEY_LEFT:
            if (menuMode) {
                if (!WindowManager__isWindowOpen(mainWindowManager)) {
                    MenuBar__selectPrevList(mainMenuBar);
                    } else {
                        WindowManager__handleLeft(mainWindowManager);
                    }
            } else {
                heldArrows[2] = true;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (menuMode) {
                if (!WindowManager__isWindowOpen(mainWindowManager)) {
                    MenuBar__selectNextList(mainMenuBar);
                } else {
                    WindowManager__handleRight(mainWindowManager);
                }
            } else {
                heldArrows[3] = true;
            }
            break;
    }
}

void readSpecialKeyboardUp(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            heldArrows[0] = false;
            break;
        case GLUT_KEY_DOWN:
            heldArrows[1] = false;
            break;
        case GLUT_KEY_LEFT:
            heldArrows[2] = false;
            break;
        case GLUT_KEY_RIGHT:
            heldArrows[3] = false;
            break;
    }
}

void readKeyboard(unsigned char key, int x, int y) {
    int lineR = Line__getColour(mainLine)[0];
    int lineG = Line__getColour(mainLine)[1];
    int lineB = Line__getColour(mainLine)[2];
    switch(key) {
        case 32:
            resetScreen();
            break;            
        case 43:
            upSize();
            break;
        case 45:
            downSize();
            break;
        case 114:
            if (lineR < 255) {
                Line__setColour(mainLine, lineR + 1, lineG, lineB);
                Overlay__setColour(mainOverlay, lineR + 1, lineG, lineB);
            }
            break;
        case 82:
            if (lineR > 0) {
                Line__setColour(mainLine, lineR - 1, lineG, lineB);
                Overlay__setColour(mainOverlay, lineR - 1, lineG, lineB);
            }
            break;
        case 103:
            if (lineG < 255) {
                Line__setColour(mainLine, lineR, lineG + 1, lineB);
                Overlay__setColour(mainOverlay, lineR, lineG + 1, lineB);
            }
            break;
        case 71:
            if (lineG > 0) {
                Line__setColour(mainLine, lineR, lineG - 1, lineB);
                Overlay__setColour(mainOverlay, lineR + 1, lineG, lineB);
            }
            break;
        case 98:
            if (lineB < 255) {
                Line__setColour(mainLine, lineR, lineG, lineB + 1);
                Overlay__setColour(mainOverlay, lineR, lineG, lineB + 1);
            }
            break;
        case 66:
            if (lineB > 0) {
                Line__setColour(mainLine, lineR, lineG, lineB - 1);
                Overlay__setColour(mainOverlay, lineR, lineG, lineB - 1);
            }
            break;
        case 13:
            menuMode = !menuMode;
            break;
        case 115:
            if (menuMode) {
                if (!WindowManager__isWindowOpen(mainWindowManager)) {
                    MenuList__callSelected(MenuBar__getSelected(mainMenuBar));
                } else {
                    WindowManager__handleAction(mainWindowManager);
                }
            } else {
                char* filename = Window__captureToFile(mainWindow);
                char* message = (char*) malloc(sizeof(char) * 256);
                snprintf(message, 256, "Saved to %s\n", filename);
                Overlay__addMessage(mainOverlay, message);
            }
            break;
    }
}

void openPane(void* _pane) {
    WindowPane* pane = (WindowPane*) _pane;
    WindowManager__openWindowPane(mainWindowManager, pane);
}

void saveScreen(void* v) {
    char* filename = Window__captureToFile(mainWindow);
    char* message = (char*) malloc(sizeof(char) * 256);
    snprintf(message, 256, "Saved to %s\n", filename);
    Overlay__addMessage(mainOverlay, message);
}

void transitionColour(void* v) {
    int tran = Line__toggleColourTransition(mainLine);
    if (tran) {
        Overlay__addMessage(mainOverlay, "Transition to next colour: On\n");
    } else {
        Overlay__addMessage(mainOverlay, "Transition to next colour: Off\n");
    }
}

bool isInit = false;

void init() {
    mainWindow = Window__create(windowWidth, windowHeight);
    mainLine = Line__create();

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


    bool redraw = callHeldInputs();
    
    if (!menuMode) {
        Line__draw(mainLine);
    }

    glFlush();
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    if (argc > 1) {
        glutInitWindowSize(320, 320);
        moveSpeed = moveSpeed / 10;        
    } else {
        glutInitWindowSize(screenWidth, screenHeight);
    }
    glutCreateWindow("OpenGL Test");
    windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    glutInitWindowPosition(50, 50);
    glutKeyboardFunc(readKeyboard);
    glutSpecialFunc(readSpecialKeyboard);
    glutSpecialUpFunc(readSpecialKeyboardUp);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
