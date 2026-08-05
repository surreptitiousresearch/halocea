#pragma once
#include <stdint.h>

/* path_heap_element — one slot of a path_state's node min-heap (4 bytes). */
typedef struct path_heap_element
{
    int16_t node_index;               /* 0x00 */
    int16_t quantized_cost_estimate;  /* 0x02 */
} path_heap_element; /* 4 bytes */
