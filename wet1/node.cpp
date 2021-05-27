#include "node.h"
#include <cassert>

Node::~Node(void) {
    if(course_data.classes) {
        assert(course_data.count > 0);
        delete [] course_data.classes;
    } else {
        assert(course_data.count == 0);
    }
}