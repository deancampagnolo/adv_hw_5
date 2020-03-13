//dcampagn - Dean Campagnolo
//aswoiski - Aaron Swoiskin

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"
#include "interp.h"

class object {
    friend class window;
    friend class mouse;
    friend class object;
private:
    shared_ptr<shape> pshape;
    vertex center;
    rgbcolor color;
public:
    object(shared_ptr<shape> a, vertex& c, rgbcolor& col, bool border);
    void draw();
    void move (GLfloat delta_x, GLfloat delta_y, int w, int h);
    bool border;
};

class mouse {
    friend class window;
private:
    int xpos {0};
    int ypos {0};
    int entered {GLUT_LEFT};
    int left_state {GLUT_UP};
    int middle_state {GLUT_UP};
    int right_state {GLUT_UP};
private:
    void set (int x, int y) { xpos = x; ypos = y; }
    void state (int button, int state);
    void draw();
};


class window {
    friend class mouse;
    friend class object;
private:
    static int moveint;
    static int width;         // in pixels
    static int height;        // in pixels
    static vector<object> objects;
    static size_t selected_obj;
    static mouse mus;
private:
    static void close();
    static void entry (int mouse_entered);
    static void display();
    static void reshape (int width, int height);
    static void keyboard (GLubyte key, int, int);
    static void special (int key, int, int);
    static void motion (int x, int y);
    static void passivemotion (int x, int y);
    static void mousefn (int button, int state, int x, int y);
public:
    static void push_back (const object& obj) {
        objects.push_back (obj); }
    static void setwidth (int width_) { width = width_; }
    static void setheight (int height_) { height = height_; }
    static void setmove (int move_) {moveint = move_;}
    static void setthick (const GLfloat& g) {thick = g;}
    static void setscolor (const rgbcolor& s) {scolor = s;}
    static void main();
public:
    static rgbcolor scolor;
    static GLfloat thick;
};

#endif
