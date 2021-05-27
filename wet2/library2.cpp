#include <memory>
#include <cassert>
#include "library2.h"
#include "DynamicArray.h"
#include "HashTable.h"
#include "RankedTree.h"

struct CourseManager {
    Node courses = Node();
    Node *classes = nullptr;
    
    s64 class_count = 0;
    
    ~CourseManager(void) {
        if(courses.capacity == 0) return;
        
        for(int i = 0; i < courses.capacity; ++i) {
            Node *it = &courses.root[i];
            while(it) {
                if(it->root) delete [] it->root;
                Node *tmp = it->next;
                if(it->prev) delete it;
                it = tmp;
            }
        }
        
        delete [] courses.root;
    };
};

// @Allocates
void *Init() {
    try {
        CourseManager *DS = new CourseManager();
        DS->courses = TableAllocate();
        return (void *)DS;
    } catch(std::bad_alloc &e) {
        return nullptr;
    }
}

// @Allocates
StatusType AddCourse(void *DS, int courseID) {
    if((DS == nullptr) || (courseID <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    
    if(TableGet(&manager->courses, courseID) != nullptr) {
        return FAILURE;
    }
    
    Node course = DynamicAllocate();
    if(course.capacity == 0) {
        return ALLOCATION_ERROR;
    }
    
    course.id = courseID;
    return TableInsert(&manager->courses, course);
}

// @Deallocates
StatusType RemoveCourse(void *DS, int courseID) {
    if((DS == nullptr) || (courseID <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    
    Node *course = TableGet(&manager->courses, courseID);
    if(course == nullptr) {
        return FAILURE;
    }
    
    for(int i = 0; i < course->count; ++i) {
        auto it = &course->root[i];
        if(it->time == 0) continue;
        RankedRemove(&manager->classes, it);
        assert(RankedFind(manager->classes, it) == nullptr);
    }
    
    return TableRemove(&manager->courses, courseID);
}

// @Allocates
StatusType AddClass(void *DS, int courseID, int *classID) {
    if((DS == nullptr) || (courseID <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    
    Node *course = TableGet(&manager->courses, courseID);
    if(course == nullptr) {
        return FAILURE;
    }
    
    *classID = course->count;
    
    s64 old_classes_root_id = -1;
    if(manager->classes && manager->classes->course_id == courseID) {
        old_classes_root_id = manager->classes->class_id;
    }
    
    Node *new_class = DynamicAppend(course);
    if(new_class == nullptr) {
        return ALLOCATION_ERROR;
    }
    
    new_class->course_id = courseID;
    new_class->class_id = *classID;
    
    if(old_classes_root_id >= 0) {
        manager->classes = DynamicGet(course, old_classes_root_id);
    }
    
    return SUCCESS;
}

StatusType WatchClass(void *DS, int courseID, int classID, int time) {
    if((DS == nullptr) || (courseID <= 0) || (classID < 0) || (time <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    Node *course = TableGet(&manager->courses, courseID);
    if(course == nullptr) {
        return FAILURE;
    }
    if(classID + 1 > course->count) {
        return INVALID_INPUT;
    }
    
    Node *the_class = DynamicGet(course, classID);
    assert(the_class);
    
    if(the_class->time > 0) {
        assert(manager->classes);
        RankedRemove(&manager->classes, the_class);
    }
    
    assert(RankedFind(manager->classes, the_class) == nullptr);
    the_class->time += time;
    assert(RankedFind(manager->classes, the_class) == nullptr);
    
    RankedInsert(&manager->classes, the_class);
    
    // std::cout << " <<<< Child count: " << manager->classes->rank << std::endl;
    
    return SUCCESS;
}

StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed) {
    if((DS == nullptr) || (courseID <= 0) || (classID < 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    Node *course = TableGet(&manager->courses, courseID);
    if(course == nullptr) {
        return FAILURE;
    }
    if(classID + 1 > course->count) {
        return INVALID_INPUT;
    }
    
    Node *the_class = DynamicGet(course, classID);
    *timeViewed = the_class->time;
    assert(*timeViewed >= 0);
    
    return SUCCESS;
}

StatusType GetIthWatchedClass(void* DS, int i, int* courseID, int* classID) {
    if((DS == nullptr) || (i <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    if((manager->classes == nullptr) || (manager->classes->rank < i)) {
        return FAILURE;
    }
    
    Node* the_class = manager->classes;
    
    while(the_class) {
        if(the_class->right) {
            if(the_class->right->rank == i - 1) {
                break;
            } else if(i <= the_class->right->rank) {
                the_class = the_class->right;
            } else {
                i -= (the_class->right->rank + 1);
                the_class = the_class->left;
            }
        } else {
            if(i == 1) break;
            
            i--;
            the_class = the_class->left;
        }
    }
    
    *courseID = the_class->course_id;
    *classID  = the_class->class_id;
    
    return SUCCESS;
}

// @Deallocates
void Quit(void **DS) {
    CourseManager *manager = static_cast<CourseManager*>(*DS);
    delete manager;
    
    *DS = nullptr;
}