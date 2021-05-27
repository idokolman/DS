#include "LinkedList.h"
#include <cassert>

Node *DllFind(Node *root, s64 id) {
    if(root == nullptr) return nullptr;
    
    Node *node = root;
    while(node->next) {
        if(node->next->id > id) {
            return node;
        }
        
        node = node->next;
    }
    
    return node;
}

inline void DllInsertAfter(Node *node, Node *insert) {
    insert->prev = node;
    insert->next = node->next;
    
    if(node->next) {
        node->next->prev = insert;
    }
    node->next = insert;
}

// @Allocates
Node *DllFindOrCreate(Node **root, s64 id) {
    if(*root == nullptr) {
        *root = new Node();
        (*root)->id = id;
        return *root;
    }
    
    Node *node = DllFind(*root, id);
    assert(node);
    
    if(node->id == id) {
        return node;
    }
    
    Node *insert = new Node();
    insert->id = id;
    DllInsertAfter(node, insert);
    
    return insert;
}

Node *DllInsert(Node **root, Node *insert) {
    if(root == nullptr) {
        *root = insert;
        return *root;
    }
    
    Node *node = DllFind(*root, insert->id);
    assert(node);
    
    if(node->id == insert->id) {
        return nullptr;
    }
    
    DllInsertAfter(node, insert);
    
    return insert;
}

Node *DllRemove(Node **root, s64 id) {
    if(*root == nullptr) return nullptr;
    Node *node = *root;
    
    while(node) {
        if(node->id > id) break;
        
        if(node->id == id) {
            if((*root)->id == id) {
                // NOTE(NJ): We are doing here some wacky stuff;
                // when removing the root from the list, we want to make the new root
                // be the previous root's next pointer, but this may lead to weird things
                // i.e. what if it had a previous pointer? should we make it the new root instead?
                // We might want to revisit it later.
                
                *root = (*root)->next;
            }
            
            if(node->prev) node->prev->next = node->next;
            if(node->next) node->next->prev = node->prev;
            
            node->next = node->prev = nullptr;
            return node;
        }
        
        node = node->next;
    }
    
    return nullptr;
}

Node *DllRemove(Node **root, Node *node) {
    if(root == nullptr || node == nullptr) return nullptr;
    return DllRemove(root, node->id);
}

Node *DllRemove(Node *node) {
    if(node == nullptr) return nullptr;
    return DllRemove(&node, node->id);
}

void DllDeleteList(Node **root) {
    if(*root == nullptr) return;
    
    // delete everything before
    {
        Node *node = (*root)->prev;
        while(node) {
            Node *tmp = node->prev;
            delete node;
            node = tmp;
        }
    }
    
    // delete everything after
    {
        Node *node = *root;
        while(node) {
            Node *tmp = node->next;
            delete node;
            node = tmp;
        }
    }
    
    *root = nullptr;
}