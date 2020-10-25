#ifndef _TEST_MM_H_
#define _TEST_MM_H_

#include "usr_lib.h"
#include "test_util.h"

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm();
extern void * memset(void * destination, int32_t c, uint64_t length);


#endif