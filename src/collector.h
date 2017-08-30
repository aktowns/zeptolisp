#ifndef __COLLECTOR_H
#define __COLLECTOR_H

#ifdef USE_GC 
#include <gc.h>

#define malloc(n) GC_malloc(n)
#define calloc(m,n) GC_malloc((m)*(n))
#define realloc(m,n) GC_realloc(m, n)
#define free(x) (0)
#endif 

#endif
