#ifndef DATA_ARRAY_H
#define DATA_ARRAY_H

#include <stdint.h>

typedef struct data_array
{
    char             name[32];
    int16_t          maximum_count;
    int16_t          size;
    uint8_t          valid;
    uint8_t          identifier_zero_invalid;
    uint8_t          _pad[2];
    uint32_t         signature;
    int16_t          first_free_absolute_index;
    int16_t          count;
    int16_t          actual_count;
    int16_t          next_identifier;
    void            *data;
} data_array;

extern void data_make_valid(data_array *data);
extern void data_make_invalid(data_array *data);

/* DATA_ARRAY_ELEMENT(arr, type, index) — address of the indexed element, reproducing the decompiler
 * idiom (char*)arr->data + arr->size * index. Byte-stride cast is required because arr->data is
 * void* and the element size varies per array; result reinterpreted as the caller's datum type.
 * The (uint16_t) truncation keeps only the absolute-index low word, so passing a full datum
 * handle is also safe — but handle callers should use DATUM_GET below, which says so.
 * Parameter named `arr`, not `data`, so it doesn't collide with the ->data field it substitutes into
 * (the preprocessor would otherwise turn arr->data into e.g. actor_data->actor_data). */
#define DATA_ARRAY_ELEMENT(arr, type, index) \
    ((type *)((char *)(arr)->data + (arr)->size * (uint16_t)(index)))

/* DATUM_GET(arr, type, datum_index) — unchecked datum-handle -> element resolve: the release-build
 * flavor of datum_get (debug builds called the checked function, see datum_get.c). A handle packs
 * the identifier salt in its high word and the absolute index in its low word (datum_index.h);
 * only the low word addresses the array, and the accessor's internal truncation is the single
 * home of the low-word extract the decompiler spelled as a cast at every inlined call site. */
#define DATUM_GET(arr, type, datum_index) DATA_ARRAY_ELEMENT(arr, type, datum_index)

/* DATA_ARRAY_IDENTIFIER(arr, index) — lvalue for the identifier word at the start of the indexed
 * element, for arrays with no known element type (e.g. clearing every slot's id on delete-all). */
#define DATA_ARRAY_IDENTIFIER(arr, index) \
    (*(int16_t *)((char *)(arr)->data + (arr)->size * (uint16_t)(index)))

#endif /* DATA_ARRAY_H */
