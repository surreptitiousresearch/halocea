#pragma once
/* objects_information — summary object-pool statistics. DB layout. */

typedef struct objects_information
{
    __int16 object_count;         /* 0x0 */
    __int16 active_object_count;  /* 0x2 */
    float   used_memory;          /* 0x4 — fraction of available object memory in use */
} objects_information;            /* 0x8 */
