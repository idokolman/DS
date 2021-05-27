#include "HashTable.h"
#include "DynamicArray.h"
#include <memory>
#include <cassert>

Node TableAllocate(s32 capacity) {
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

s64 GetHash(Node *table, s64 id) {
    return id % table->capacity; // TODO(NJ): Better Hash Function
}

void TableInsertAux(Node *table, Node insert) {
    s64 hash = GetHash(table, insert.id);
    insert.prev = insert.next = nullptr;
    
    if(table->root[hash].id == -1) {
        assert(table->root[hash].root == nullptr);
        table->root[hash] = insert;
    } else {
        Node *node = &table->root[hash];
        assert(node->id != insert.id);
        while(node->next) {
            assert(node->id != insert.id);
            node = node->next;
        }
        
        insert.prev = node;
        node->next = new Node(insert);
    }
    
    table->count += 1;
}

StatusType TableReallocate(Node *table, s64 new_capacity) {
    try {
        Node new_table = TableAllocate(new_capacity);
        
        for(s64 i = 0; i < table->capacity; ++i) {
            Node *it = &table->root[i];
            if(it->id == -1) continue;
            
            while(it) {
                Node *tmp = it->next;
                TableInsertAux(&new_table, *it);
                if(it->prev) {
                    delete it;
                }
                it = tmp;
            }
        }
        
        delete [] table->root;
        table->root = new_table.root;
        table->capacity = new_table.capacity;
    } catch(std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    
    return SUCCESS;
}

StatusType TableInsert(Node *table, Node insert) {
    TableInsertAux(table, insert);
    
    if(table->count >= ((3 * table->capacity) / 4)) {
        return TableReallocate(table, table->capacity * 2);
    }
    
    return SUCCESS;
}

StatusType TableInsert(Node *table, s64 id) {
    Node insert;
    insert.id = id;
    return TableInsert(table, insert);
}

StatusType TableRemove(Node *table, s64 id) {
    s64 hash = GetHash(table, id);
    if(table->root[hash].id == -1) return FAILURE;
    Node *it = &table->root[hash];
    do {
        if(it->id == id) {
            it->id = -1;
            if(it->root) {
                delete [] it->root;
                it->root = nullptr;
            }
            
            if(it->prev) {
                it->prev->next = it->next;
                if(it->next) it->next->prev = it->prev;
                delete it;
            } else if(it->next) {
                Node *tmp = it->next;
                *it = *it->next;
                if(it->next) it->next->prev = it;
                delete tmp;
                it->prev = nullptr;
            }
            
            break;
        }
        it = it->next;
    } while(it);
    
    table->count -= 1;
    if(table->count <= (table->capacity / 4)) {
        if((table->capacity/4) > 4) {
            return TableReallocate(table, table->capacity / 2);
        }
    }
    
    return SUCCESS;
}

Node *TableGet(Node *table, s64 id) {
    s64 hash = GetHash(table, id);
    if(table->root[hash].id == -1) return nullptr;
    
    Node *it = &table->root[hash];
    do {
        if(it->id == id) {
            return it;
        }
        it = it->next;
    } while(it);
    
    return nullptr;
}
