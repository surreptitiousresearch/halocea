#pragma once
/* dynamic_array — generic growable array used across the Blam engine */

typedef struct dynamic_array
{
    int   element_size;  /* 0x0 */
    int   count;         /* 0x4 */
    void *elements;      /* 0x8 */
} dynamic_array;

#ifdef __cplusplus
extern "C" {
#endif
extern void dynamic_array_new(dynamic_array *array, int element_size);
extern void dynamic_array_delete(dynamic_array *array);
extern int dynamic_array_add_element(dynamic_array *array);  /* returns index or -1 */
#ifdef __cplusplus
}
#endif
