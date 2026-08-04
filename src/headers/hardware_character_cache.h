#pragma once
// DB-verified via types_members _CC578FC7FF706676A8856431CFF27EE6 (anonymous PDB type)

#include "hardware_character.h"
#include "bitmap_data.h"

typedef struct hardware_character_cache_struct
{
    unsigned char initialized;        // 0x00
    unsigned char _pad01;             // 0x01
    short read_index;                 // 0x02
    short write_index;                // 0x04
    short x0;                         // 0x06
    short y0;                         // 0x08
    short maximum_character_height;   // 0x0A
    bitmap_data *bitmap;              // 0x0C
    hardware_character characters[512]; // 0x10
} hardware_character_cache_struct;

extern hardware_character_cache_struct hardware_character_cache;
