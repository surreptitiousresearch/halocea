#pragma once

#include "point2d.h"

/* 48-byte bitmap descriptor. */
typedef struct bitmap_data
{
    unsigned int signature;       // 0x00 'bitm' live tag
    short width;                  // 0x04
    short height;                 // 0x06
    short depth;                  // 0x08
    short type;                   // 0x0A
    short format;                 // 0x0C
    unsigned short flags;         // 0x0E
    point2d registration_point;   // 0x10
    short mipmap_count;           // 0x14
    short mipmap_pad;             // 0x16
    int pixels_offset;            // 0x18
    int pixels_size;              // 0x1C
    int tag_index;                // 0x20
    int cache_block_index;        // 0x24
    void *hardware_format;        // 0x28
    void *base_address;           // 0x2C
} bitmap_data;
