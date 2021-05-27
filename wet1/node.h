#ifndef _GENERAL_NODE_H
#define _GENERAL_NODE_H

#include <stdint.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

struct Class_Data {
    struct Node *time;
};

struct Course_Data {
    s32 count;
    struct Node *classes;
    struct Node *first_in_time0;
    struct Node *last_in_time0;
};

struct Time_Data {
    struct Node *root;
    struct Node *first;
    struct Node *last ;
};

struct Node {
    union {
        s64 id = -1;
        struct {
            s32 class_id;
            s32 course_id;
        };
    };
    
    // AVL Tree stuff {
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
    s64 height = 1;
    // }
    
    // Linked list stuff {
    Node *next = nullptr;
    Node *prev  = nullptr;
    // }
    
    Class_Data class_data = {};
    Course_Data course_data = {};
    Time_Data time_data = {};
    
    ~Node(void);
};

#endif //_GENERAL_NODE_H
