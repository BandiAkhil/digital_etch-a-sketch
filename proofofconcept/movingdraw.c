#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
//Library for (up)loading images
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <png.h>
#include <stdint.h>

#define GL_GLEXT_PROTOTYPES 1

static GLubyte *pixels = NULL;
static const GLenum FORMAT = GL_RGBA;
static const GLuint FORMAT_NBYTES = 4;
static unsigned int nscreenshots = 0;

float moveSpeed = 0.0003f;

bool heldArrows[4];

float offsetsUpRight[] = { 0.0f, 0.0f };
float circleSize = 0.01f;
float sizeIncrease = 0.002f;

int colors[] = {0,0,0};

bool reset = true;
bool menuMode = false;

int screenWidth;
int screenHeight;

int windowWidth;
int windowHeight;

GLuint image;

void onLeft() {
    if (offsetsUpRight[1] > -1.0f){
        offsetsUpRight[1] = offsetsUpRight[1] - moveSpeed;
    }
}

void onRight() {
    if (offsetsUpRight[1] < 1.0f){
        offsetsUpRight[1] = offsetsUpRight[1] + moveSpeed;
    }
}

void onUp() {
    if (offsetsUpRight[0] < 1.0f){
        offsetsUpRight[0] = offsetsUpRight[0] + moveSpeed;
    }
}

void onDown() {
    if (offsetsUpRight[0] > -1.0f){
        offsetsUpRight[0] = offsetsUpRight[0] - moveSpeed;
    }
}

void changeColors(int red, int green, int blue) {
    colors[0] = red;
    colors[1] = green;
    colors[2] = blue;
}

//Increase the size of the dot
void upSize() {
    if (circleSize < 0.8f){
        circleSize += sizeIncrease;
    }
}

//Decrease the size of the dot
void downSize() {
    if (circleSize > 0.0f){
        circleSize -= sizeIncrease;
    }
}

bool callHeldInputs() {
    bool hasChanged = false;
    if (heldArrows[0]) {
        hasChanged = true;
        onUp();
    }
    if (heldArrows[1]) {
        hasChanged = true;
        onDown();
    }
    if (heldArrows[2]) {
        hasChanged = true;
        onLeft();
    }
    if (heldArrows[3]) {
        hasChanged = true;
        onRight();
    }
    return hasChanged;
}

void resetScreen() {
    reset = true;
}

void readSpecialKeyboard(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            heldArrows[0] = true;
            break;
        case GLUT_KEY_DOWN:
            heldArrows[1] = true;
            break;
        case GLUT_KEY_LEFT:
            heldArrows[2] = true;
            break;
        case GLUT_KEY_RIGHT:
            heldArrows[3] = true;
            break;
    }
}

void readSpecialKeyboardUp(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            heldArrows[0] = false;
            break;
        case GLUT_KEY_DOWN:
            heldArrows[1] = false;
            break;
        case GLUT_KEY_LEFT:
            heldArrows[2] = false;
            break;
        case GLUT_KEY_RIGHT:
            heldArrows[3] = false;
            break;
    }
}

static int save_png_to_file(GLubyte *pixels, const char *path){
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte ** row_pointers = NULL;

    int status = -1;
    int pixel_size = 3;
    int depth = 8;

    int max_filename = 256;
    char filename[max_filename];
    snprintf(filename, max_filename, "%s%d.png", path, nscreenshots);
    fp = fopen(filename, "wb");
    nscreenshots++;
    if (! fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }

    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }

    /* Set up error handling. */

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }

    /* Set image attributes. */

    png_set_IHDR (png_ptr,
                  info_ptr,
                  windowWidth,
                  windowHeight,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */

    row_pointers = png_malloc (png_ptr, windowHeight * sizeof (png_byte *));
    for (int y = 0; y < windowHeight; y++) {
        png_byte *row =
                png_malloc (png_ptr, sizeof (uint8_t) * windowWidth * pixel_size);
        row_pointers[y] = row;
        for (int x = 0; x < windowWidth; x++) {
            size_t cur = FORMAT_NBYTES * ((windowHeight - y - 1) * windowWidth + x);
            *(row++) = pixels[cur];
            *(row++) = pixels[cur + 1];
            *(row++) = pixels[cur + 2];
        }
    }

    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status = 0;

    for (int y = 0; y < windowHeight; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);

    png_failure:
    png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
    png_create_write_struct_failed:
    fclose (fp);
    fopen_failed:
    return status;
}

int createPNG(){
    int status = 0;

    if (! pixels){
        return -1;
    }

    glReadPixels(0, 0, windowWidth, windowHeight, FORMAT, GL_UNSIGNED_BYTE, pixels);
    if (save_png_to_file(pixels, "saves/save")){
        fprintf(stderr, "Error writing file.\n");
        status = -1;
    }

    return status;
}

void readKeyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 32:
            resetScreen();
            break;            
        case 43:
            upSize();
            break;
        case 45:
            downSize();
            break;
        case 114:
            if (colors[0] < 255){
                changeColors(colors[0] + 1, colors[1], colors[2]);
            }
            break;
        case 82:
            if (colors[0] > 0){
                changeColors(colors[0] - 1, colors[1], colors[2]);
            }
            break;
        case 103:
            if (colors[1] < 255){
                changeColors(colors[0], colors[1] + 1, colors[2]);
            }
            break;
        case 71:
            if (colors[1] > 0){
                changeColors(colors[0], colors[1] - 1, colors[2]);
            }
            break;
        case 98:
            if (colors[2] < 255){
                changeColors(colors[0], colors[1], colors[2] + 1);
            }
            break;
        case 66:
            if (colors[2] > 0){
                changeColors(colors[0], colors[1], colors[2] - 1);
            }
            break;
        case 13:
            menuMode = !menuMode;
            break;
        case 112:
            createPNG();
            break;
    }
}

//Upload image to GLuint texture
void uploadImage(){
    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* upload = SOIL_load_image("loading.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, upload);

    glBindTexture(GL_TEXTURE_2D, 0);
}

//Draw the image given in the texture file
void drawImage(){
    uploadImage();
    glBindTexture(GL_TEXTURE_2D, image);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(-1.0f, -1.0f);
    glTexCoord2i(0, 1); glVertex2i(-1.0f, 1.0f);
    glTexCoord2i(1, 1); glVertex2i(1.0f, 1.0f);
    glTexCoord2i(1, 0); glVertex2i(1.0f, -1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//Draw the cicle with given parameters (size, color, position)
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    glColor3ub(colors[0],colors[1],colors[2]);
    glLineWidth(circleSize);
    for(int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * (float)(ii) / (float)(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
}

bool isInit = false;

void init() {
    glMatrixMode(GL_PROJECTION);
    float aspect = (float)windowWidth / (float)windowHeight;
    glOrtho(-aspect, aspect, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    pixels = malloc(FORMAT_NBYTES * windowWidth * windowHeight);
}

bool pixelsLoaded = false;
GLuint pixelsTex;

void display() {
    if (!isInit) {
        init();
        isInit = true;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    if (reset) {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    reset = false;

    if (menuMode) {
        if (!pixelsLoaded) {
            glGenTextures(1, &pixelsTex);
            glBindTexture(GL_TEXTURE_2D, pixelsTex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, windowWidth, windowHeight, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            pixelsLoaded = true;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBindTexture(GL_TEXTURE_2D, pixelsTex);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-1.0f, -1.0f);
        glTexCoord2i(0, 1); glVertex2i(-1.0f, 1.0f);
        glTexCoord2i(1, 1); glVertex2i(1.0f, 1.0f);
        glTexCoord2i(1, 0); glVertex2i(1.0f, -1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = (float)windowWidth / (float)windowHeight;
        glOrtho(-aspect, aspect, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    } else if (pixelsLoaded) {
        //reset texture
        pixelsLoaded = false;
    }


    bool redraw = callHeldInputs();

    /*glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-squareSize[0] + offsetsUpRight[1], -squareSize[1] + offsetsUpRight[0]);
        glVertex2f(squareSize[0] + offsetsUpRight[1], -squareSize[1] + offsetsUpRight[0]);
        glVertex2f(squareSize[0] + offsetsUpRight[1], squareSize[1] + offsetsUpRight[0]);
        glVertex2f(-squareSize[0] + offsetsUpRight[1], squareSize[1] + offsetsUpRight[0]);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);*/
    drawCircle(0.0f + offsetsUpRight[1], 0.0f + offsetsUpRight[0], circleSize, 20);

    glFlush();
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    if (argc > 1) {
        glutInitWindowSize(320, 320);
        moveSpeed = moveSpeed / 10;        
    } else {
        glutInitWindowSize(screenWidth, screenHeight);
    }
    glutCreateWindow("OpenGL Test");
    windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    glutInitWindowPosition(50, 50);
    glutKeyboardFunc(readKeyboard);
    glutSpecialFunc(readSpecialKeyboard);
    glutSpecialUpFunc(readSpecialKeyboardUp);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
