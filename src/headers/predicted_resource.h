#pragma once
/* predicted_resource — one element of a tag's predicted_resources block (8 bytes; DB-verified). */

typedef struct predicted_resource
{
    __int16 type;           /* 0x00 — 0 = bitmap, 1 = sound */
    __int16 resource_index; /* 0x02 — index within the tag's resource block */
    int     tag_index;      /* 0x04 */
} predicted_resource;       /* 8 bytes */
