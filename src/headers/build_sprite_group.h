#pragma once
/* build_sprite_group — one vertex-buffer batch within a build_sprite_data (16 bytes, DB layout). */

typedef struct bitmap_data bitmap_data;

typedef struct build_sprite_group
{
    int            vertex_buffer_index; /* 0x0 */
    void          *vertices;            /* 0x4 */
    __int16        sprite_count;        /* 0x8 */
    char           pad_0xA[2];          /* 0xA */
    bitmap_data   *bitmap;              /* 0xC */
} build_sprite_group;                    /* 16 bytes */
