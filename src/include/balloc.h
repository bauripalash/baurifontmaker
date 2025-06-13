#ifndef BALLOC_H
#define BALLOC_H

#include <stddef.h>
#include <string.h>

// #define BALLOC_MOCK_ALLOC_FAIL
#define BALLOC_MOCK_ALLOC_LIMIT 1350

void *balloc(size_t sz);
void *bcalloc(size_t n, size_t sz);
void *brealloc(void *ptr, size_t nsz);
void bfree(void *ptr);

// #define BALLOC_IMPL

#ifdef BALLOC_IMPL

#include <stdio.h>
#include <stdlib.h>

static size_t alloc_total_bytes = 0;
static size_t alloc_total_blocks = 0;

static inline void *ballocImpl(size_t size, const char *f, int l) {
    void *ptr = NULL;
    printf("Mem -> %zu\n", alloc_total_bytes);
#ifdef BALLOC_MOCK_ALLOC_FAIL
    if (alloc_total_bytes < BALLOC_MOCK_ALLOC_LIMIT) {
        ptr = malloc(size);
    }
#else
    ptr = malloc(size);
#endif

    if (!ptr) {
#ifdef BALLOC_DEBUG
        fprintf(stderr, "[MEMERR] [%s:%d] malloc(%zu) failed.\n", f, l, size);
#endif
        return NULL;
    } else {
        alloc_total_bytes += size;
        alloc_total_blocks += 1;
    }

    return ptr;
}

static inline void *bcallocImpl(size_t n, size_t size, const char *f, int l) {
    void *ptr = calloc(n, size);

    if (!ptr) {

#ifdef BALLOC_DEBUG
        fprintf(
            stderr, "[MEMERR] [%s:%d] calloc(%zu, %zu) failed.\n", f, l, n, size
        );
#endif
        return NULL;
    } else {
        alloc_total_bytes += size;
        alloc_total_blocks += 1;
    }

    return ptr;
}

static inline void *
breallocImpl(void *oldptr, size_t size, const char *f, int l) {
    if (!oldptr) {
        return ballocImpl(size, f, l);
    }

    void *ptr = realloc(oldptr, size);

    if (!ptr) {
#ifdef BALLOC_DEBUG
        fprintf(
            stderr, "[MEMERR] [%s, %d] realloc(%p, %zu) failed.\n", f, l,
            oldptr, size
        );
#endif
        return NULL;
    } else {
        alloc_total_bytes += size;
        alloc_total_blocks += 1;
    }

    return ptr;
}

static inline void bfreeImpl(void *ptr, const char *f, int l) {
    if (!ptr) {
        return;
    }

    free(ptr);

    if (alloc_total_blocks >= 1) {
        alloc_total_bytes -= 1;
    }
}

void MemStats(void) {
    printf(
        "[bAlloc Stats] = [%zu blocks] [%zu bytes]\n", alloc_total_blocks,
        alloc_total_bytes
    );
}

void *balloc(size_t sz) { return ballocImpl(sz, __FILE__, __LINE__); }

void *bcalloc(size_t n, size_t sz) {
    return bcallocImpl(n, sz, __FILE__, __LINE__);
}

void *brealloc(void *ptr, size_t nsz) {
    return breallocImpl(ptr, nsz, __FILE__, __LINE__);
}

void bfree(void *ptr) { bfreeImpl(ptr, __FILE__, __LINE__); }

#endif

#endif
