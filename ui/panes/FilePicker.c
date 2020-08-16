#include <GL/glut.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "FilePicker.h"
#include "Style.h"
#include "UIHelpers.h"
#include "WindowManager.h"

static char* selectedFile = "";
static int selectConfirm = 0;
static int selectedIndex = 0;
static int fileCount = 0;
static float padding = 0.2f;
static Window* mainWindow = NULL;

/*private*/ void drawFile(WindowPane* pane, Window* window, int index, char* file) {
    int* selected = WindowPane__getSelected(pane);
    float* ortho = WindowPane__getOrthoLimits(pane);
    //draw highlight
    if ((index == selected[1] && !selectConfirm) || selectConfirm && selectedIndex == index) {
        selectedFile = file;
        selectedIndex = index;
        glColor3ub(mainColoursLight[0], mainColoursLight[1], mainColoursLight[2]);
        glBegin(GL_QUADS);
            glVertex2f(ortho[0] + padding, ortho[3] - barHeight * (index + 1));
            glVertex2f(ortho[0] + padding, ortho[3] - barHeight * index);
            glVertex2f(ortho[1] - padding, ortho[3] - barHeight * index);
            glVertex2f(ortho[1] - padding, ortho[3] - barHeight * (index + 1));
        glEnd();
    } 
    
    //draw text
    int textHeight = getBitmapHeight();
    float relativeTextHeight = (float)textHeight / (float)Window__getHeight(window);
    glColor3ub(0, 0, 0);
    drawBitmapText(file,
        ortho[0] + padding,
        ortho[3] - barHeight * (index + 1) + relativeTextHeight * 0);
}

void FilePicker__drawFunc(WindowPane* pane, Window* window) {
    if (WindowPane__isReset(pane)) {
        selectedFile = "";
        selectConfirm = 0;
        selectedIndex = 0;
        fileCount = 0;
        mainWindow = NULL;
    }

    mainWindow = window;
    DIR *dir;
    struct dirent *ent;
    fileCount = 0;
    if ((dir = opendir ("../saves")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) {
                drawFile(pane, window, fileCount, ent->d_name);
                fileCount++;
            }
        }
        closedir (dir);
        // set the max select
        if (!selectConfirm) {
            WindowPane__setYLimits(pane, 0, fileCount - 1);
        }
    } else {
        /* could not open directory */
        perror ("what\n");
        return;
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

    //draw load button
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

    int loadWidth = getBitmapWidth("Load");
    float relativeLoadWidth = (float)loadWidth / (float)Window__getWidth(window) * aspect;
    glColor3ub(0, 0, 0);
    drawBitmapText("Load",
        ortho[1] - padding - buttonWidth / 2 - relativeLoadWidth,
        ortho[2] + padding / 3 + buttonHeight / 2 - relativeTextHeight / 2);

}

void FilePicker__handleActionFunc(WindowPane* pane) {
    int* selected = WindowPane__getSelected(pane);

    if (selectConfirm) {
        //handle cancel
        if (selected[0] == 0) {
            selectConfirm = 0;
            WindowPane__setXLimits(pane, 0, 0);
            WindowPane__setSelected(pane, 0, selected[1]);
            selected[1] = 0;
        }
    
        //handle load
        if (selected[0] == 1) {
            if (mainWindow != NULL) {
                //printf("LOADING FILE: %s\n", selectedFile);
                char filePath[256];
                snprintf(filePath, 256, "../saves/%s", selectedFile);
                Window__loadFromFile(mainWindow, filePath);
                WindowManager__closeWindowPane(WindowPane__getManager(pane));
            }
        }
    } else {
        //handle file select
        if (selected[1] >= 0) {
            //printf("SELECTED FILE=%s\n", selectedFile);
            selectConfirm = 1;        
            WindowPane__setXLimits(pane, 0, 1);
            WindowPane__setSelected(pane, 0, selected[1]);
        }
    }
}

WindowPane* FilePicker__createPane() {
    WindowPane* result = WindowPane__create(1.0f, 1.0f, "Load File");
    WindowPane__setDraw(result, FilePicker__drawFunc);
    WindowPane__setHandleAction(result, FilePicker__handleActionFunc);
    WindowPane__setXLimits(result, 0, 0);
    return result;
}
