#ifndef _TEST_MM_H_
#define _TEST_MM_H_

#include "usr_lib.h"
#include "test_util.h"

#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 * 1024 * 120 //Should be around 80% of memory managed by the MM
#define NULL (void *) 0

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void memtest(int argc, char **argv);

#endif