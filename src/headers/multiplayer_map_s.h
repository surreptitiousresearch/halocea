#pragma once
/* multiplayer_map_s — one entry in the known multiplayer map list (map_list.c) */

#include "buildin_multiplayer_map_s.h"

typedef struct multiplayer_map_s
{
    char *name;            /* 0x0 — heap-allocated, lowercased map name */
    int   pic_name_index;  /* 0x4 — index into the loading-picture name table */
} multiplayer_map_s; /* 8 bytes */
