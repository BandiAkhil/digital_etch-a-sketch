#ifndef FILEPICKER_H
#define FILEPICKER_H
#include "WindowPane.h"
#include "Window.h"

void FilePicker__drawFunc(WindowPane* pane, Window* window);

void FilePicker__handleActionFunc(WindowPane* pane);

WindowPane* FilePicker__createPane();

#endif
