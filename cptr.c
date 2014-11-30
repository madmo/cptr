#include "cptr.h"

#include <stdlib.h>

typedef struct _cptr cptr;
typedef struct _cptr_list cptr_list;

struct _cptr_list {
	cptr* head;
	cptr* tail;
};

struct _cptr {
	cptr* next;
	cptr* prev;

	unsigned int refCount;
	cptr_list children;

	void* data;
};

static void addptr(cptr_list* l, cptr* t)
{
	if (l->tail) {
		l->tail->next = t;
		t->prev = l->tail;
	} else {
		l->head = t;
		t->prev = NULL;
	}

	l->tail = t;
	t->next = NULL;
}

void* cptr_alloc(size_t size)
{
	cptr* p;
	p = malloc(sizeof(cptr) + size);
	p->data = ((char*)p) + sizeof(cptr);
	p->refCount = 1;
	return p->data;
}

void* cptr_retain(void* ptr, void* parent)
{
	cptr* p;
	p = (cptr*)(((char*)ptr) - sizeof(cptr));
	p->refCount++;

	if (parent) {
		cptr* pp = (cptr*)(((char*)parent) - sizeof(cptr));
		addptr(&pp->children, ptr);
	}

	return ptr;
}

void cptr_release(void* ptr)
{
	cptr* p;
	p = (cptr*)(((char*)ptr) - sizeof(cptr));
	p->refCount--;

	if (p->refCount == 0) {
		void* c = p->children.head;

		while (c) {
			cptr_release(c);
		}

		free(ptr);
	}
}
