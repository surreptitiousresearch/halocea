#pragma once
/* scenario_structure_bsp_reference — one entry in the scenario's structure-BSP list (32 bytes). The
 * leading offset/size/address triple locates the BSP block in the cache file and its load destination;
 * the embedded structure_bsp tag reference (index at +0x1C) identifies the tag. */

#include "tag_reference.h"

typedef struct scenario_structure_bsp_reference
{
    int           offset;                                   /* 0x00 — file offset in the cache */
    int           size;                                     /* 0x04 — byte size of the BSP block */
    void         *address;                                  /* 0x08 — load destination */
    unsigned int  unused[1];                                /* 0x0C */
    tag_reference structure_bsp;                            /* 0x10 — tag reference (index at +0x1C) */
} scenario_structure_bsp_reference;                         /* 32 bytes */
