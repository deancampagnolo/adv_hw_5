#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"
#include "graphics.h"

unordered_map<string,interpreter::interpreterfn> interpreter::interp_map {
    {"define" , &interpreter::do_define },
    {"draw"   , &interpreter::do_draw   },
    {"border" , &interpreter::do_border },
    {"moveby" , &interpreter::do_moveby },
};

unordered_map<string,interpreter::factoryfn> interpreter::factory_map {
    {"text"     , &interpreter::make_text     },
    {"ellipse"  , &interpreter::make_ellipse  },
    {"circle"   , &interpreter::make_circle   },
    {"polygon"  , &interpreter::make_polygon  },
    {"rectangle", &interpreter::make_rectangle},
    {"square"   , &interpreter::make_square   },
    {"triangle" , &interpreter::make_triangle },
    {"equilateral" , &interpreter::make_equilateral },
    {"diamond"   , &interpreter::make_diamond },
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
    for (const auto& itor: objmap) {
        cout << "objmap[" << itor.first << "] = "
        << *itor.second << endl;
    }
}

void interpreter::interpret (const parameters& params) {
    DEBUGF ('i', params);
    param begin = params.cbegin();
    string command = *begin;
    auto itor = interp_map.find (command);
    if (itor == interp_map.end()) throw runtime_error ("syntax error");
    interpreterfn func = itor->second;
    func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    string name = *begin;
    objmap.insert ({name, make_shape (++begin, end)});
}

void interpreter::do_draw (param begin, param end) { 
    DEBUGF ('f', range (begin, end));
    if (end - begin != 4) throw runtime_error ("syntax error");
    string name = begin[1];
    shape_map::const_iterator itor = objmap.find (name);
    if (itor == objmap.end()) {
        throw runtime_error (name + ": no such shape");
    }
    rgbcolor color {begin[0]};
    vertex where {from_string<GLfloat> (begin[2]),
        from_string<GLfloat> (begin[3])};
    window::push_back (object (itor->second, where, color, false));
}
void interpreter::do_moveby (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    if (end - begin != 1) throw runtime_error ("syntax error");
    int pmove = stoi(begin[0]);
    window::setmove(pmove);
    
    // change a variable in graphics that will change speed
    // make a global variable
}
void interpreter::do_border (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    if (end - begin != 2) throw runtime_error ("syntax error");
    rgbcolor color {begin[0]};
    GLfloat Dthick = from_string<GLfloat> (begin[1]);
    window::setthick(Dthick);
    window::setscolor(color);
}
shape_ptr interpreter::make_shape (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    string type = *begin++;
    auto itor = factory_map.find(type);
    if (itor == factory_map.end()) {
        throw runtime_error (type + ": no such shape");
    }
    factoryfn func = itor->second;
    return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    auto the_font = fontcode.find(begin[0])->second;
    ++begin;
    string words = "";
    while(begin != end){
        words.append(*begin + " ");
        ++begin;
    }
    return make_shared<text> (the_font, words);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    return make_shared<ellipse> (from_string<GLfloat> (begin[0]), from_string<GLfloat> (begin[1]));
}

shape_ptr interpreter::make_circle (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    return make_shared<circle> (from_string<GLfloat> (begin[0]));
}
// the triangles
shape_ptr interpreter::make_triangle (param begin, param end){
    if (end - begin != 6) throw runtime_error ("syntax error");
    vertex_list points;
    auto itor = begin;
    float y_sum = 0.0, x_sum = 0.0;
    while(itor != end){
        vertex temp;
        temp.xpos = from_string<GLfloat> (*itor);
        x_sum += from_string<GLfloat> (*itor);
        ++itor;
        temp.ypos = from_string<GLfloat> (*itor);
        y_sum += from_string<GLfloat> (*itor);
        ++itor; 
        points.push_back(temp); 
    }
    int x_avg = x_sum/3;
    int y_avg = y_sum/3;

    for (auto vert : points) {
        vert.x -= x_avg;
        vert.y -= y_avg;
    }

    return make_shared<triangle> (polyPoints);
}
//diamond
shape_ptr interpreter::make_diamond (param begin, param end) {
DEBUGF ('f', range (begin, end));

return make_shared<diamond> (from_string<GLfloat> (begin[0]), from_string<GLfloat> (begin[1]));
}

//equilateral triangle
shape_ptr interpreter::make_equilateral (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    return make_shared<equilateral> (from_string<GLfloat> (begin[0]));
}
shape_ptr interpreter::make_polygon (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    vertex_list polyPoints;
    //insert the vectors from the front
    auto itor = begin;
    //unsigned long int i = 1;
    //adding all vector cooridinates to vector list
    float y = 0.0, x = 0.0, total = 0.0;
    while(itor != end){
        
        x += from_string<GLfloat> (*itor);
        itor++;
        y += from_string<GLfloat> (*itor);
        if(itor == end) throw runtime_error(" incorrect number of vectors");
        // vertex ass = new vertex( x , y );
        total += 1.0;
        itor++;
        
    }
    x= x/total;
    y= y/total;
    itor = begin;
    while(itor != end){
        vertex temp;
        temp.xpos = from_string<GLfloat> (*itor) - x;
        itor++;
        temp.ypos = from_string<GLfloat> (*itor) - y;
        
        polyPoints.push_back(temp);
        itor++;
    }
    return make_shared<polygon> (polyPoints);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    GLfloat width = from_string<GLfloat> (begin[0]);
    GLfloat length = from_string<GLfloat> (begin[1]);
    return make_shared<rectangle> (width,length );
}

shape_ptr interpreter::make_square (param begin, param end) {
    DEBUGF ('f', range (begin, end));
    return make_shared<square> (from_string<GLfloat> (begin[0]));
}
