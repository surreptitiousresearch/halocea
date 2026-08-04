#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */

#include "bitmap_data.h"

/* texture_cache_texture — datum of pc_texture_cache_globals.textures (16-byte stride).
   No DB type exists for this record; the layout is recovered from its two call sites
   (texture_cache_start_loading_bitmap / texture_cache_delete_block_proc). */
typedef struct texture_cache_texture
{
    short                  identifier;          /* 0x00 — data_array datum identifier */
    short                  read_request_handle; /* 0x02 — async cache_file_read handle */
    volatile unsigned char finished;            /* 0x04 — read-finished flag */
    unsigned char          pad[3];              /* 0x05 */
    bitmap_data           *bitmap;              /* 0x08 — owning bitmap */
    void                  *hardware_format;     /* 0x0C — copy of bitmap->hardware_format */
} texture_cache_texture;                        /* 16 bytes */
