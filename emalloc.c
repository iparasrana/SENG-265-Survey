#include <stdlib.h>
#include <stdio.h>
#include "emalloc.h"

/* allocate given memory size of heap in bytes */
void *emalloc(size_t n) {
    void *p; 

    p = malloc(n);
    if (p == NULL) { // if memory allocation failed, display error and exit
        fprintf(stderr, "malloc of %zu bytes failed", n); 
        exit(1);
    }   

    return p;
}
