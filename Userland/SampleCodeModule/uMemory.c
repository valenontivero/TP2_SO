#include <stddef.h>
#include <stdint.h>
#include <usyscalls.h>

void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    return sys_malloc((uint64_t)size);
}

void free(void *ptr) {
    if (ptr != NULL) {
        sys_free(ptr);
    }
}

