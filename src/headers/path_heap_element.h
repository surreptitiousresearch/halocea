#pragma once

/* path_heap_element — one slot of a path_state's node min-heap (4 bytes). */
typedef struct path_heap_element
{
    __int16 node_index;               /* 0x00 */
    __int16 quantized_cost_estimate;  /* 0x02 */
} path_heap_element; /* 4 bytes */
