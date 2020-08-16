#ifndef OVERLAY_H
#define OVERLAY_H
#include "Window.h"

struct Overlay;

typedef struct Overlay Overlay;

const static int messageAliveCycles = 500;

const static int messageMax = 10;

void Overlay__init(Overlay* self);

Overlay* Overlay__create();

void Overlay__setColour(Overlay* self, int r, int g, int b);

void Overlay__setThickness(Overlay* self, float t);

void Overlay__addMessage(Overlay* self, char* message);

void Overlay__removeMessage(Overlay* self);

void Overlay__draw(Overlay* self, Window* mainWindow);

#endif
