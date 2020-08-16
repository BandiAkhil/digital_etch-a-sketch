#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "Window.h"

struct Window {
    int width;
    int height;
    GLuint texture;
    /*private*/ int screenshotN;
};

typedef struct Window Window;

/*private*/ void setOrtho(Window* self) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)self->width / (float)self->height;
    glOrtho(-aspect, aspect, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Window__init(Window* self, int width, int height) {
    GLuint texture;
    self->width = width;
    self->height = height;
    self->texture = texture;
    self->screenshotN = 0;

    setOrtho(self);
};

Window* Window__create(int width, int height) {
    Window* result = (Window*) malloc(sizeof(Window));        
    Window__init(result, width, height);
    return result;
};

void Window__captureTexture(Window* self) {
    glGenTextures(1, &self->texture);

    glBindTexture(GL_TEXTURE_2D, self->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, self->width, self->height, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
};

void Window__drawTexture(Window* self) {
    float aspect = (float)self->width / (float)self->height;

    glBindTexture(GL_TEXTURE_2D, self->texture);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2f(-aspect, -1.0f);
    glTexCoord2i(0, 1); glVertex2f(-aspect, 1.0f);
    glTexCoord2i(1, 1); glVertex2f(aspect, 1.0f);
    glTexCoord2i(1, 0); glVertex2f(aspect, -1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    setOrtho(self);
};

/*private*/ char* pixelsToPNG(Window* self, GLubyte *pixels, const char *path){
    int max_filename = 256;
    char* filename = (char*) malloc(sizeof(char) * max_filename);
    snprintf(filename, max_filename, "%s%d.bmp", path, self->screenshotN);
    while (access(filename, F_OK) != -1) {
        self->screenshotN = self->screenshotN + 1;
        snprintf(filename, max_filename, "%s%d.bmp", path, self->screenshotN);
    }

    SOIL_save_image(
        filename,
        SOIL_SAVE_TYPE_BMP,
        self->width, self->height, 4,
        pixels
    );
    return filename;
}

char* Window__captureToFile(Window* self) {
    //printf("w: %i, h: %i\n", self->width, self->height);
    char* pixels = malloc(sizeof(char) * 4 * self->width * self->height);
    glReadPixels(0, 0, self->width, self->height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    char* result = pixelsToPNG(self, pixels, "../saves/save");
    self->screenshotN = self->screenshotN + 1;
    free(pixels);
    return result;
};

void Window__loadFromFile(Window* self, char* filePath) {
    /*
    printf("TRYING TO LOAD FROM %s\n", filePath);
    self->texture = SOIL_load_OGL_texture(
        "./test11.bmp",
        SOIL_LOAD_RGBA, 
        SOIL_CREATE_NEW_ID, 
        0
    );
    if (self->texture == 0) {
        printf("something wrong, %s\n", SOIL_last_result());
        
        int channels = 4;
        int imgSize = channels * self->width * self->height;
        printf("size: %i\n", imgSize);
        
        char* image = SOIL_load_image("./test11.bmp", &self->width, &self->height, &channels, SOIL_LOAD_RGBA) ;
        
        self->texture = SOIL_create_OGL_texture(image, self->width, self->height, 4,  SOIL_CREATE_NEW_ID, 0);
        if (self->texture == 0) {
            printf("something wrong 2, electric boogaloo, %s\n", SOIL_last_result());
        }
    }*/
};

int Window__getWidth(Window* self) {
    return self->width;
}

int Window__getHeight(Window* self) {
    return self->height;
}
