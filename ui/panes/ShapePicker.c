#include <GL/glut.h>
#include <stdio.h>
#include "Line.h"
#include "UIHelpers.h"
#include "ShapePicker.h"
#include "WindowPane.h"
#include "WindowManager.h"
#include "Shape.h"
#include "Style.h"

static Shape** shapeList;

static Line* mainLine = NULL;

static Shape* selectedShape = NULL;
static int selectConfirm = 0;
static int selectedIndex = 0;
static int shapeCount = 0;
static float padding = 0.2f;
static float itemPaddingSide = 0.05f;
static float itemPaddingTop = 0.01f;
static float shapeR = 0.04f;
static Window* mainWindow = NULL;

/*private*/ void drawShape(WindowPane* pane, Window* window, int index, Shape* shape) {
    int* selected = WindowPane__getSelected(pane);
    float* ortho = WindowPane__getOrthoLimits(pane);

    float itemHeight = shapeR * 2 + itemPaddingTop * 2;
    int textHeight = getBitmapHeight();
    float relativeTextHeight = (float)textHeight / (float)Window__getHeight(window);

    //draw highlight
    if ((index == selected[1] && !selectConfirm) || selectConfirm && selectedIndex == index) {
        selectedShape = shape;
        selectedIndex = index;
        glColor3ub(mainColoursLight[0], mainColoursLight[1], mainColoursLight[2]);
        glBegin(GL_QUADS);
            glVertex2f(ortho[0] + padding, ortho[3] - itemHeight * (index + 1));
            glVertex2f(ortho[0] + padding, ortho[3] - itemHeight * index);
            glVertex2f(ortho[1] - padding, ortho[3] - itemHeight * index);
            glVertex2f(ortho[1] - padding, ortho[3] - itemHeight * (index + 1));
        glEnd();
    }

    //draw shape
    glColor3ub(0, 0, 0);
    Shape__draw(shape, ortho[0] + padding + itemPaddingSide + shapeR, ortho[3] - itemHeight * (index + 1) + (itemHeight / 2), shapeR);

    //draw text
    drawBitmapText(Shape__getName(shape), ortho[0] + padding + itemPaddingSide * 2 + shapeR * 2, ortho[3] - itemHeight * (index + 1) + (itemHeight / 2) - relativeTextHeight / 2);
}

void ShapePicker__drawFunc(WindowPane* pane, Window* window) {
    if (WindowPane__isReset(pane)) {
        selectedShape = NULL;
        selectConfirm = 0;
        selectedIndex = 0;
        mainWindow = NULL;
    }

    mainWindow = window;

    //draw the shapes
    for (int i = 0; i < shapeCount; i++) {
        drawShape(pane, window, i, shapeList[i]);
    }

    int* selected = WindowPane__getSelected(pane);
    float* ortho = WindowPane__getOrthoLimits(pane);
    float aspect = (float)Window__getWidth(window) / (float)Window__getHeight(window);

    float buttonHeight = padding / 3;
    float buttonWidth = (ortho[1] - ortho[0] - padding * 2) / 3;

    //draw cancel button
    if (selectConfirm && selected[0] == 0) {
        glColor3ub(mainColoursLight[0], mainColoursLight[1], mainColoursLight[2]);
    } else {
        glColor3ub(mainColoursDark[0], mainColoursDark[1], mainColoursDark[2]);
    }
    glBegin(GL_QUADS);
        glVertex2f(ortho[0] + padding, ortho[2] + padding / 3);
        glVertex2f(ortho[0] + padding, ortho[2] + padding / 3 + buttonHeight);
        glVertex2f(ortho[0] + padding + buttonWidth, ortho[2] + padding / 3 + buttonHeight);
        glVertex2f(ortho[0] + padding + buttonWidth, ortho[2] + padding / 3);
    glEnd();

    int cancelWidth = getBitmapWidth("Cancel");
    int textHeight = getBitmapHeight();
    float relativeCancelWidth = (float)cancelWidth / (float)Window__getWidth(window) * aspect;
    float relativeTextHeight = (float)textHeight / (float)Window__getHeight(window);
    glColor3ub(0, 0, 0);
    drawBitmapText("Cancel",
        ortho[0] + padding + buttonWidth / 2 - relativeCancelWidth,
        ortho[2] + padding / 3 + buttonHeight / 2 - relativeTextHeight / 2);

    //draw confirm button
    if (selectConfirm && selected[0] == 1) {
        glColor3ub(mainColoursLight[0], mainColoursLight[1], mainColoursLight[2]);
    } else {
        glColor3ub(mainColoursDark[0], mainColoursDark[1], mainColoursDark[2]);
    }
    glBegin(GL_QUADS);
        glVertex2f(ortho[1] - padding - buttonWidth, ortho[2] + padding / 3);
        glVertex2f(ortho[1] - padding - buttonWidth, ortho[2] + padding / 3 + buttonHeight);
        glVertex2f(ortho[1] - padding, ortho[2] + padding / 3 + buttonHeight);
        glVertex2f(ortho[1] - padding, ortho[2] + padding / 3);
    glEnd();

    int loadWidth = getBitmapWidth("Confirm");
    float relativeLoadWidth = (float)loadWidth / (float)Window__getWidth(window) * aspect;
    glColor3ub(0, 0, 0);
    drawBitmapText("Confirm",
        ortho[1] - padding - buttonWidth / 2 - relativeLoadWidth,
        ortho[2] + padding / 3 + buttonHeight / 2 - relativeTextHeight / 2);


    glColor3f(1.0f, 1.0f, 1.0f);
}

void ShapePicker__handleActionFunc(WindowPane* pane) {
    int* selected = WindowPane__getSelected(pane);

    if (selectConfirm) {
        //handle cancel
        if (selected[0] == 0) {
            selectConfirm = 0;
            WindowPane__setXLimits(pane, 0, 0);
            WindowPane__setSelected(pane, 0, selected[1]);
            selected[1] = 0;
        }
    
        //handle confirm
        if (selected[0] == 1) {
            if (mainLine != NULL) {
                Line__setShape(mainLine, selectedShape);
                WindowManager__closeWindowPane(WindowPane__getManager(pane));
            }
        }
    } else {
        //handle file select
        if (selected[1] >= 0) {
            selectConfirm = 1;        
            WindowPane__setXLimits(pane, 0, 1);
            WindowPane__setSelected(pane, 0, selected[1]);
        }
    }
}

WindowPane* ShapePicker__createPane(Line* line) {
    mainLine = line;
    shapeCount = 4;
    shapeList = (Shape**) malloc(sizeof(Shape*) * shapeCount);
    shapeList[0] = Shape__create(drawCircle, "Circle");
    shapeList[1] = Shape__create(drawStar, "Star");
    shapeList[2] = Shape__create(drawSquare, "Square");
    shapeList[3] = Shape__create(drawTriangle, "Triangle");
    WindowPane* result = WindowPane__create(1.0f, 1.0f, "Change Shape");
    WindowPane__setDraw(result, ShapePicker__drawFunc);
    WindowPane__setHandleAction(result, ShapePicker__handleActionFunc);
    WindowPane__setXLimits(result, 0, 0);
    WindowPane__setYLimits(result, 0, shapeCount - 1);
    return result;
}
