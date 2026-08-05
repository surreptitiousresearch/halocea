#pragma once
/* font_header — the runtime tag definition for a 'font' tag (156 bytes). Layout from the database. */

#include <stdint.h>
#include "tag_block.h"
#include "tag_reference.h"
#include "tag_data.h"

typedef struct font_header
{
    unsigned int  flags;                /* 0x00 */
    int16_t       ascending_height;     /* 0x04 */
    int16_t       descending_height;    /* 0x06 */
    int16_t       leading_height;       /* 0x08 */
    int16_t       leading_width;        /* 0x0A */
    int           pad[9];               /* 0x0C */
    tag_block     character_tables;     /* 0x30 */
    tag_reference style_fonts[4];       /* 0x3C */
    tag_block     characters;           /* 0x7C */
    tag_data      pixels;               /* 0x88 */
} font_header;                          /* 156 bytes */
