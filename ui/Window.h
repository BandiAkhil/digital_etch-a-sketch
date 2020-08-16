#ifndef WINDOW_H
#define WINDOW_H

struct Window;

typedef struct Window Window;

void Window__init(Window* self, int width, int height);

Window* Window__create(int width, int height);

void Window__captureTexture(Window* self);

void Window__drawTexture(Window* self);

char* Window__captureToFile(Window* self);

void Window__loadFromFile(Window* self, char* filePath);

int Window__getWidth(Window* self);

int Window__getHeight(Window* self);

#endif
