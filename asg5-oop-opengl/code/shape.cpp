//dcampagn - Dean Campagnolo
//aswoiski - Aaron Swoiskin
#include <typeinfo>
#include <unordered_map>
using namespace std;
#include "graphics.h"
#include "shape.h"
#include "util.h"
#include "math.h"

ostream& operator<< (ostream& out, const vertex& where) {
    out << "(" << where.xpos << "," << where.ypos << ")";
    return out;
}

shape::shape() {
    DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font_, const string& textdata_):
      glut_bitmap_font(glut_bitmap_font_), textdata(textdata_) {
    DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
    DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
    DEBUGF ('c', this);
}

polygon::polygon (const vertex_list& vertices_): vertices(vertices_) {
    DEBUGF ('c', this);
}

triangle::triangle (const vertex_list& verticies_) : polygon(verticies_){
    DEBUGF ('c', this);
}

equilateral::equilateral (GLfloat width): 
triangle ({{-width/2, -((sqrt(3)/2)*width)/2}, 
            {width/2, -((sqrt(3)/2)*width)/2}, 
            {0, ((sqrt(3)/2)*width)/2}}) {
    DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height):
polygon({{-width/2, -height/2}, {width/2, -height/2}, 
        {width/2, height/2}, {-width/2, height/2}}) {
    DEBUGF ('c', this << "(" << width << "," << height << ")");
}

diamond::diamond (GLfloat width, GLfloat height):
polygon({{0.0, -height/2}, {width/2, 0.0}, {0.0, height/2}, {-width/2, 0.0}}) {
    DEBUGF ('c', this << "(" << width << "," << height << ")");
}

square::square (GLfloat width): rectangle (width, width) {
    DEBUGF ('c', this);
}

void text::draw (const vertex& center, const rgbcolor& color, bool border) const {
    DEBUGF ('d', this << "(" << center << ",");
    glColor3ubv(color.ubvec);
    glRasterPos2f (center.xpos, center.ypos);
    auto newtext = reinterpret_cast<const unsigned char*> (textdata.c_str());
    glutBitmapString (glut_bitmap_font, newtext);
}

void ellipse::draw (const vertex& center, const rgbcolor& color, bool border) const {
    DEBUGF ('d', this << "(" << center << "," << color << ")");
    glBegin (GL_POLYGON);
    glEnable (GL_LINE_SMOOTH);
    glColor3ubv (color.ubvec);
    float delta = 2 * M_PI / 32;
    float width = dimension.xpos;
    float height = dimension.ypos;
    for (float theta = 0; theta < 2 * M_PI; theta += delta) {
        float xpos = width * cos (theta) + center.xpos;
        float ypos = height * sin (theta) + center.ypos;
        glVertex2f (xpos, ypos);
    }
    glEnd();
    
    if(border == true){
        glLineWidth(window::thick);
        glBegin(GL_LINES);
        glEnable(GL_LINE_SMOOTH);    
        glColor3ubv (window::scolor.ubvec);
        for (float theta = 0; theta < 2 * M_PI; theta += delta) {
            float xposy = width * cos (theta) + center.xpos;
            float yposy = height * sin (theta) + center.ypos;
            glVertex2d (xposy, yposy);
        }   
        glEnd();
    }
}

void polygon::draw (const vertex& center, const rgbcolor& color, bool border) const {
    DEBUGF ('d', this << "(" << center << "," << color << ")");
    glBegin (GL_POLYGON);
    glEnable(GL_LINE_SMOOTH);
    glColor3ubv (color.ubvec);

    for(auto vert : vertices) {
        glVertex2f (vert.xpos + center.xpos, vert.ypos + center.ypos);
    }
    glEnd();
    if(border == true){ 
        glLineWidth(window::thick);
        glBegin(GL_LINES);
        glEnable(GL_LINE_SMOOTH);
        
        glColor3ubv (window::scolor.ubvec);
        for(auto vert : vertices){
            glVertex2d (vert.xpos + center.xpos, vert.ypos + center.ypos);
        }
        glEnd();  
   }
}

void shape::show (ostream& out) const {
    out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
    shape::show (out);
    out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
    << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
    shape::show (out);
    out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
    shape::show (out);
    out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
    obj.show (out);
    return out;
}

