#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "UIHelpers.h"

int getBitmapWidth(char* string) {
    char *c;
    int result = 0;

    for (c=string; *c != (char) 0; c++) {
        result += glutBitmapWidth(GLUT_BITMAP_8_BY_13, *c);
        //printf("c: %c, w: %i\n", *c, glutBitmapWidth(GLUT_BITMAP_8_BY_13, *c));
    }
    return result;
}

int getBitmapHeight() {
    return 13;
}

void drawBitmapText(char* string, float x, float y) {
    char *c;
    glRasterPos2f(x, y);

    for (c=string; *c != (char) 0; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

const static int num_segments = 20;

void drawCircle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * (float)(ii) / (float)(num_segments);//get the current angle 
        float x = r * cosf(theta);//calculate the x component 
        float y = r * sinf(theta);//calculate the y component 
        glVertex2f(x + cx, y + cy);//output vertex 
    }
    glEnd();
}

const static int numPoints = 5;

void drawStar(float cx, float cy, float r) {
    float DegToRad = 3.14159 / 180;

    glBegin (GL_TRIANGLE_FAN);
    int count = 1;
    glVertex2f(cx, cy);
    for (int i = 0; i <= numPoints*2; i++) {
        float DegInRad = i * 360.0/(numPoints*2) * DegToRad - 54 * DegToRad;
        if(count%2!=0)
            glVertex2d (cx + cos (DegInRad) * r, cy + sin (DegInRad) * r);
        else
            glVertex2d ((cx + cos (DegInRad) * r/2), (cy + sin (DegInRad) * r/2));
        count++;
    }
    glEnd();
}

void drawSquare(float cx, float cy, float r) {
    glBegin(GL_QUADS);
        glVertex2f(cx - r, cy - r);
        glVertex2f(cx - r, cy + r);
        glVertex2f(cx + r, cy + r);
        glVertex2f(cx + r, cy - r);
    glEnd();
}

void drawTriangle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < 3; ii++) {
        float theta = 2.0f * 3.1415926f * (float)(ii) / (float)(3) + (0.5f * 3.1415926f);//get the current angle 
        float x = r * cosf(theta);//calculate the x component 
        float y = r * sinf(theta);//calculate the y component 
        glVertex2f(x + cx, y + cy);//output vertex 
    }
    glEnd();

}
