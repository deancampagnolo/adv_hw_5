//dcampagn - Dean Campagnolo
//aswoiski - Aaron Swoiskin
#include <cmath>
#include <typeinfo>
#include <unordered_map>
#include <libgen.h>
using namespace std;
#include "graphics.h"
#include "shape.h"
#include "util.h"
#include "math.h"

static unordered_map<void*,string> fontname {
    {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
    {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
    {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
    {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
    {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
    {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
    {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

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

void text::draw (const vertex& center, const rgbcolor& color, bool b) const {
    DEBUGF ('d', this << "(" << center << "," << b<< ")");
    
    glColor3ubv(color.ubvec);
    glRasterPos2f (center.xpos, center.ypos);
    for (auto ch: textdata) glutBitmapCharacter (glut_bitmap_font, ch);
    glutSwapBuffers();
}

void ellipse::draw (const vertex& center, const rgbcolor& color, bool b) const {
    DEBUGF ('d', this << "(" << center << "," << color << ")");
    glBegin (GL_POLYGON);
   glEnable (GL_LINE_SMOOTH);
   glColor3ubv (color.ubvec);
   const float delta = 2 * M_PI / 32;
   float width = dimension.xpos;
   float height = dimension.ypos;
   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      float xposy = width * cos (theta) + center.xpos;
      float yposy = height * sin (theta) + center.ypos;
      glVertex2f (xposy, yposy);
   }
   glEnd();
   if(b == true){
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

void polygon::draw (const vertex& center, const rgbcolor& color, bool b) const {
    DEBUGF ('d', this << "(" << center << "," << color << ")");
    //glColor3uv(color.ubvec);
     glBegin (GL_POLYGON);
     glEnable(GL_LINE_SMOOTH);
   glColor3ubv (color.ubvec);
   //constexpr GLfloat third = 1.0 / 3.0;
  /* float total = 0.0;
   float x = center.xpos, y = center.ypos;
   for( auto a: vertices){
        x += a.xpos;
        y += a.ypos;
        total+=1.0;
   
   } 
   x = x/total;
   y = y/total; 
   */
   for(auto p: vertices)glVertex2f (p.xpos+center.xpos, p.ypos+center.ypos);
   
   glEnd();
   if(b == true){ 
         glLineWidth(window::thick);
         glBegin(GL_LINES);
           glEnable(GL_LINE_SMOOTH);
          
           glColor3ubv (window::scolor.ubvec);
         for(auto p: vertices){
         glVertex2d (p.xpos+center.xpos, p.ypos+center.ypos);
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
