#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "node.h"

Node *DllFind(Node *root, s64 id);
Node *DllFindOrCreate(Node **root, s64 id); // @Allocates
Node *DllInsert(Node **root, Node *node);

Node *DllRemove(Node **root, s64 id);
Node *DllRemove(Node **root, Node *node);

Node *DllRemove(Node *node);

void DllDeleteList(Node **root); // @Deallocates

#endif //_LINKED_LIST_H
