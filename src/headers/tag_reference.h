#pragma once
/* tag_reference — a reference to another tag by group + name, resolved to an index */
typedef struct tag_reference
{
    unsigned int group_tag;   /* 0x0 */
    char        *name;        /* 0x4 */
    int          name_length; /* 0x8 */
    int          index;       /* 0xC — resolved tag index, -1 if unresolved */
} tag_reference;              /* 16 bytes */
