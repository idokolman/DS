#ifndef _DYNAMIC_ARRAY_H
#define _DYNAMIC_ARRAY_H

#include "node.h"

Node DynamicAllocate(s32 capacity = 4);
Node *DynamicAppend(Node *array);
Node *DynamicGet(Node *array, s32 index);

#endif //_DYNAMIC_ARRAY_H
