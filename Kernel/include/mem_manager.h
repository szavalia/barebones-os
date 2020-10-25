#ifndef _MEM_MANAGER_
#define _MEM_MANAGER_
    #include <stddef.h>
    #include <stdint.h>
    void * ltmalloc(size_t size);
    void ltmfree(void * pointer);
    void printMemList();

    extern uint8_t memContent(uint8_t * i);

    typedef struct filedescriptor_t{
        int num;
        void * pt;
        char lock;
    }filedescriptor_t;
#endif