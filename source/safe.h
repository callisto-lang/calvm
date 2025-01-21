#ifndef VM_SAFE_H
#define VM_SAFE_H

#include "common.h"

#define NEW(T) (T*) SafeMalloc(sizeof(T))

void* SafeMalloc(size_t size);
void* SafeRealloc(void* ptr, size_t size);

#endif
