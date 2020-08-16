#include <GL/glut.h>
#include "Shape.h"

struct Shape {
    void (*drawFunc)(float, float, float);
    char* name;
};

typedef struct Shape Shape;

void Shape__init(Shape* self, void (*drawFunc)(float, float, float), char* name) {
    self->drawFunc = drawFunc;
    self->name = name;
}

Shape* Shape__create(void (*drawFunc)(float, float, float), char* name) {
    Shape* shape = (Shape*) malloc(sizeof(Shape));
    Shape__init(shape, drawFunc, name);
    return shape;
}

void Shape__draw(Shape* self, float cx, float cy, float size) {
    self->drawFunc(cx, cy, size);
}

char* Shape__getName(Shape* self) {
    return self->name;
}
