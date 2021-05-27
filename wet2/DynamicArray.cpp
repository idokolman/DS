#include "DynamicArray.h"
#include <memory>
#include <memory.h>
#include <cassert>

Node DynamicAllocate(s32 capacity) {
    try {
        Node array;
        array.root = new Node[capacity];
        array.capacity = capacity;
        array.count = 0;
        
        return array;
    } catch(std::bad_alloc &e) {
        return Node();
    }
}

Node *DynamicAppend(Node *array) {
    array->count += 1;
    if(array->count >= array->capacity) {
        assert(array->count == array->capacity);
        try {
            s64 new_capacity = array->capacity * 2;
            
            Node *new_root = new Node[new_capacity];
            
            for(s64 i = 0; i < array->count; ++i) {
                Node *it = &array->root[i];
                if(it->parent) {
                    if(it->parent->left == it) {
                        it->parent->left = &new_root[i];
                    } else {
                        assert(it->parent->right == it);
                        it->parent->right = &new_root[i];
                    }
                }
                if(it->left) {
                    it->left->parent = &new_root[i];
                }
                if(it->right) {
                    it->right->parent = &new_root[i];
                }
                
                new_root[i] = *it;
            }
            
            delete [] array->root;
            array->capacity = new_capacity;
            array->root = new_root;
        } catch(std::bad_alloc &e) {
            return nullptr;
        }
    }
    Node *node = &array->root[array->count - 1];
    
    return node;
}

Node *DynamicGet(Node *array, s32 index) {
    if(index >= array->count) return nullptr;
    return &array->root[index];
}