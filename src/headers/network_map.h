#pragma once
/* network_map — the map identity carried in a network game (132 bytes). */

typedef struct network_map
{
    unsigned int version;  /* 0x00 */
    char         name[128]; /* 0x04 */
} network_map;
