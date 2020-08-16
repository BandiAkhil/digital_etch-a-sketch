#ifndef SHAPE_H
#define SHAPE_H

struct Shape;

typedef struct Shape Shape;

void Shape__init(Shape* self, void (*drawFunc)(float, float, float), char* name);

Shape* Shape__create(void (*drawFunc)(float, float, float), char* name);

void Shape__draw(Shape* self, float cx, float cy, float size);

char* Shape__getName(Shape* self);

#endif
