#ifndef CPTR_H
#define CPTR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* cptr_alloc(size_t size);
void* cptr_retain(void* ptr, void* parent);
void cptr_release(void* ptr);

#ifdef __cplusplus
}
#endif

#endif
