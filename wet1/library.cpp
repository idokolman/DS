#include <memory>
#include <cassert>
#include "library.h"
#include "LinkedList.h"
#include "AvlTree.h"

// Why don't we do a "class CourseManager"
// with all of the member functions and what's not?
// Well, that would imply that we need to write the API two times,
// maintain it two times, and be twice less porductive.
// So, we'll do the DOP (Data Oriented Programming) thing, and treat
// the data as, well data, and not some abstract actors in a weird
// abstract world. Our actions change the data.

struct CourseManager {
    Node *courses   = nullptr;
    Node *times     = nullptr;
    Node *last_time = nullptr;
    
    s64 class_count = 0;
    
    Node *courses_in_time0 = nullptr;
    
    ~CourseManager(void) {
        DllDeleteList(&times);
        AvlDeleteTree(&courses);
        AvlDeleteTree(&courses_in_time0);
    };
};

#ifndef NDEBUG
int DEBUGAssertManagerIsHealthyAux(Node* node){
    if (node==nullptr){
        return 0;
    }
    int count = node->course_data.count;
    count += DEBUGAssertManagerIsHealthyAux(node->left);
    count += DEBUGAssertManagerIsHealthyAux(node->right);
    return count;
}
#endif

inline void DEBUGAssertManagerIsHealthy(CourseManager *manager) {
#ifndef NDEBUG
    int numOfClasses = manager->class_count;
    
    int count0 = DEBUGAssertManagerIsHealthyAux(manager->courses);
    count0 += DEBUGAssertManagerIsHealthyAux(manager->courses_in_time0);
    assert(count0 == numOfClasses);
    
    Node *the_time;
    
    int count2 = 0;
    the_time = manager->times;
    assert(the_time->prev == nullptr);
    while(the_time) {
        Node *the_class = the_time->time_data.last;
        while(the_class) {
            count2 += 1;
            the_class = the_class->prev;
        }
        
        the_time = the_time->next;
    }
    
    assert(count2 == numOfClasses);
    
    int count1 = 0;
    the_time = manager->last_time;
    assert(the_time->next == nullptr);
    
    while(the_time) {
        Node *the_class = the_time->time_data.first;
        while(the_class) {
            count1 += 1;
            the_class = the_class->next;
        }
        
        the_time = the_time->prev;
    }
    
    assert(count1 == numOfClasses);
#endif
}

void DeleteTime(CourseManager *manager, Node *time) {
    assert(time->id <= manager->last_time->id);
    if(manager->last_time == time) {
        manager->last_time = time->prev;
    }
    time = DllRemove(time);
    delete time;
}

// @Allocates
void *Init() {
    try {
        CourseManager *DS = new CourseManager();
        DS->times = new Node();
        DS->times->id = 0;
        DS->last_time = DS->times;
        
        return (void *)DS;
    } catch(std::bad_alloc &e) {
        return nullptr;
    }
}

// @Allocates
StatusType AddCourse(void *DS, int courseID, int numOfClasses) {
    if((DS == nullptr) || (courseID <= 0) || (numOfClasses <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    
    try {
        if(AvlFind(manager->courses, courseID).found) return FAILURE;
        
        Node *course = new Node();
        course->id = courseID;
        
        Avl_Result result = AvlInsert(&manager->courses_in_time0, course);
        
        if(result.found || result.result == nullptr) {
            delete course;
            return FAILURE;
        }
        Course_Data *course_data = &course->course_data;
        course_data->count = numOfClasses;
        course_data->classes = new Node[course_data->count];
        
        for(int i = 0; i < course_data->count; ++i) {
            Node *it = (course_data->classes + i);
            it->course_id = courseID;
            it->class_id  = i;
            it->class_data.time = manager->times;
            if(i > 0) {
                it->prev = &course_data->classes[i - 1];
            }
            if(i < course_data->count - 1) {
                it->next = &course_data->classes[i + 1];
            }
        }
        course_data->first_in_time0 = &course_data->classes[0];
        course_data->last_in_time0  = &course_data->classes[numOfClasses - 1];
        
        // NOTE(NJ): Find previous/next course places in time0
        // and insert our new list between them.
        if(result.prev) {
            result.prev->course_data.last_in_time0->next = course_data->first_in_time0;
            course_data->first_in_time0->prev = result.prev->course_data.last_in_time0;
        } else {
            manager->times->time_data.first = course_data->first_in_time0;
        }
        
        if(result.next) {
            result.next->course_data.first_in_time0->prev = course_data->last_in_time0;
            course_data->last_in_time0->next = result.next->course_data.first_in_time0;
        } else {
            manager->times->time_data.last = course_data->last_in_time0;
        }
        
        manager->class_count += course->course_data.count;
    } catch(std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    
    DEBUGAssertManagerIsHealthy(manager);
    
    return SUCCESS;
}

// @Deallocates
StatusType RemoveCourse(void *DS, int courseID) {
    if((DS == nullptr) || (courseID <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    
    Avl_Result result = AvlRemove(&manager->courses, courseID);
    if(!result.found) {
        result = AvlRemove(&manager->courses_in_time0, courseID);
    }
    if(!result.found) {
        return FAILURE;
    }
    
    Node *course = result.result;
    Course_Data *course_data = &course->course_data;
    
    for(int i = 0; i < course_data->count; ++i) {
        Node *it = (course_data->classes + i);
        Node *time = it->class_data.time;
        
        if(time->time_data.first == it) {
            time->time_data.first = it->next;
        }
        if(time->time_data.last == it) {
            time->time_data.last = it->prev;
        }
        
        if(time->id > 0) {
            AvlRemove(&time->time_data.root, it);
            if(time->time_data.root == nullptr) {
                DeleteTime(manager, time);
            }
        }
        
        DllRemove(it);
    }
    
    manager->class_count -= course_data->count;
    assert(manager->class_count >= 0);
    assert(course_data->classes);
    
    delete course;
    
    DEBUGAssertManagerIsHealthy(manager);
    
    return SUCCESS;
}

// @Allocates/Deallocates
StatusType WatchClass(void *DS, int courseID, int classID, int time) {
    if((DS == nullptr) || (courseID <= 0) || (classID < 0) || (time <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    
    Avl_Result result = AvlFind(manager->courses, courseID);
    if(!result.found) {
        result = AvlFind(manager->courses_in_time0, courseID);
    }
    if(!result.found) {
        return FAILURE;
    }
    
    Node *course = result.result;
    if(classID >= course->course_data.count) {
        return INVALID_INPUT;
    }
    
    Node *the_class = &course->course_data.classes[classID];
    Node *current_time = the_class->class_data.time;
    s32 time_id = current_time->id;
    assert(current_time);
    DEBUGAssertManagerIsHealthy(manager);
    
    if(time_id == 0) {
        if((course->course_data.first_in_time0 == the_class) &&
           (course->course_data.last_in_time0 == the_class)) {
            AvlRemove(&manager->courses_in_time0, courseID);
            course->course_data.last_in_time0  = nullptr;
            course->course_data.first_in_time0 = nullptr;
            AvlInsert(&manager->courses, course);
        } else {
            if(course->course_data.last_in_time0 == the_class) {
                course->course_data.last_in_time0 = the_class->prev;
            }
            if(course->course_data.first_in_time0 == the_class) {
                course->course_data.first_in_time0 = the_class->next;
            }
        }
    } else {
        AvlRemove(&current_time->time_data.root, the_class);
    }
    if(current_time->time_data.first == the_class) {
        current_time->time_data.first = the_class->next;
    }
    if(current_time->time_data.last == the_class) {
        current_time->time_data.last = the_class->prev;
    }
    DllRemove(the_class);
    
    try {
        Node *new_time = DllFindOrCreate(&current_time, time_id + time);
        if(new_time->time_data.root == nullptr) {
            new_time->time_data.root  = the_class;
            new_time->time_data.first = the_class;
            new_time->time_data.last  = the_class;
        } else {
            Avl_Result result = AvlInsert(&new_time->time_data.root, the_class);
            assert(!result.found);
            
            the_class->next = result.next;
            the_class->prev = result.prev;
            
            if(result.prev) result.prev->next = the_class;
            if(result.next) result.next->prev = the_class;
            
            if(the_class->id < new_time->time_data.first->id) {
                new_time->time_data.first = the_class;
            }
            if(the_class->id > new_time->time_data.last->id) {
                new_time->time_data.last = the_class;
            }
        }
        
        if(new_time->next == nullptr) {
            manager->last_time = new_time;
        }
        the_class->class_data.time = new_time;
        
        // If our current_time is empty, remove it from the list and delete it.
        if(time_id > 0 && current_time->time_data.root == nullptr) {
            DeleteTime(manager, current_time);
        }
    } catch(std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    
    DEBUGAssertManagerIsHealthy(manager);
    
    return SUCCESS;
}

StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed) {
    if((DS == nullptr) || (courseID <= 0) || (classID < 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    
    Avl_Result result = AvlFind(manager->courses, courseID);
    if(!result.found) {
        result = AvlFind(manager->courses_in_time0, courseID);
    }
    
    if(!result.found) {
        return FAILURE;
    }
    
    Node *course = result.result;
    if(classID >= course->course_data.count) {
        return INVALID_INPUT;
    }
    
    Node *the_class = &course->course_data.classes[classID];
    assert(the_class->class_data.time);
    
    *timeViewed = the_class->class_data.time->id;
    assert(*timeViewed >= 0);
    
    return SUCCESS;
}

StatusType GetMostViewedClasses(void *DS, int numOfClasses, int *courses, int *classes) {
    if((DS == nullptr) || (numOfClasses <= 0)) {
        return INVALID_INPUT;
    }
    
    CourseManager *manager = static_cast<CourseManager*>(DS);
    if(manager->class_count < numOfClasses) {
        return FAILURE;
    }
    
    int count = 0;
    Node *time = manager->last_time;
    assert(time->next == nullptr);
    
    while(time) {
        Node *the_class = time->time_data.first;
        while(the_class && count < numOfClasses) {
            courses[count] = the_class->course_id;
            classes[count] = the_class->class_id;
            count += 1;
            
            the_class = the_class->next;
        }
        
        if(count >= numOfClasses) {
            break;
        }
        
        time = time->prev;
    }
    
    assert(count == numOfClasses);
    
    return SUCCESS;
}

// @Deallocates
void Quit(void **DS) {
    CourseManager *manager = static_cast<CourseManager*>(*DS);
    delete manager;
    
    *DS = nullptr;
}