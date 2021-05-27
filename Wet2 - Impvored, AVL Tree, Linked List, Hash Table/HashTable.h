#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include "node.h"
#include "library2.h"

Node TableAllocate(s32 capacity = 4);
StatusType TableInsert(Node *table, Node insert);
StatusType TableInsert(Node *table, s64 id);
StatusType TableRemove(Node *table, s64 id);
Node *TableGet(Node *table, s64 id);

#endif //_HASH_TABLE_H
