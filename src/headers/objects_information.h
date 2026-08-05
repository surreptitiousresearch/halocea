#pragma once
#include <stdint.h>
/* objects_information — summary object-pool statistics. DB layout. */

typedef struct objects_information
{
    int16_t object_count;         /* 0x0 */
    int16_t active_object_count;  /* 0x2 */
    float   used_memory;          /* 0x4 — fraction of available object memory in use */
} objects_information;            /* 0x8 */
