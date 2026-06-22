#include <stdlib.h>

typedef struct {
    int cnt;
    int cap;
} ArrayHeader;

#define array_t(t) t *

#define arr_header(arr) (((ArrayHeader*)(arr))-1)

#define arr_count(arr) (arr_header(arr)->cnt)
#define arr_capacity(arr) (arr_header(arr)->cap)

#define arr_init(arr) do {                                              \
    ArrayHeader *header = malloc(sizeof(ArrayHeader)+sizeof(*(arr))*2); \
    header->cnt = 0;                                                    \
    header->cap = 2;                                                    \
    (arr) = (void *)(header+1);                                         \
} while (0)

#define arr_free(arr) do {                          \
    ArrayHeader *header = arr_header(arr);          \
    free(header);                                   \
    arr = NULL;                                     \
} while (0)

#define arr_delete(arr, from, to) do {                  \
    ArrayHeader *header = arr_header(arr);              \
    if (from >= to || from < 0 || to > header->cnt) {   \
        break;                                          \
    }                                                   \
    int i, n = to - from;                               \
    for (i = from; i < header->cnt-n; i += 1) {         \
        (arr)[i] = (arr)[to + i - from];                \
    }                                                   \
    header->cnt -= n;                                   \
} while(0)

/*
 * The realloc function may cause the array pointer
 * to become NULL. It is necessary to perform an
 * additional check by yourself.
 */
#define arr_push(arr, value) do {                                                   \
/* The realloc function may cause the array pointer                                 \
 * to become NULL. It is necessary to perform an                                    \
 * additional check by yourself.                                                    \
 */                                                                                 \
    ArrayHeader *header = arr_header(arr);                                          \
    (arr)[header->cnt] = (value);                                                   \
    header->cnt += 1;                                                               \
    if (header->cnt >= header->cap) {                                               \
        header-> cap = header->cap * 15 / 10;                                       \
        header = realloc(header, sizeof(ArrayHeader)+sizeof(*(arr))*header->cap);   \
        (arr) = (void *)(header+1);                                                 \
    }                                                                               \
}  while (0)
