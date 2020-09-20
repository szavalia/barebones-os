#ifndef _MEM_MANAGER_
#define _MEM_MANAGER_
    #include <stddef.h>
    void * ltmalloc(size_t size);
    void ltmfree(void * pointer);
#endif