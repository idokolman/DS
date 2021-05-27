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

struct Node {
    Node() : id(-1) {};
    
    union {
        s64 id;
        struct {
            s32 class_id;
            s32 course_id;
        };
    };
    
    s32 time = 0;
    
    // Hash table stuff {
    Node *next = nullptr;
    Node *prev = nullptr;
    // }
    
    // Dynamic Array stuff {
    Node *root = nullptr;
    s32 capacity = -1;
    s32 count = 0;
    // }
    
    // Rank Tree stuff {
    Node *parent = nullptr;
    Node *left  = nullptr;
    Node *right = nullptr;
    s64 height  = 1;
    s64 rank    = 1;
    // }
};

#endif //_GENERAL_NODE_H
