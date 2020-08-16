#ifndef SHAPEPICKER_H
#define SHAPEPICKER_H
#include "WindowPane.h"

void ShapePicker__drawFunc(WindowPane* pane, Window* window);

void ShapePicker__handleActionFunc(WindowPane* pane);

WindowPane* ShapePicker__createPane();

#endif
