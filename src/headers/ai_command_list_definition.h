#pragma once
/* ai_command_list_definition — one scenario AI "command list" (scripted order sequence) tag block
 * element (96 bytes). Layout transcribed verbatim from the database (types_members). */

#include "tag_block.h"

typedef struct ai_command_list_definition
{
    char             name[32];                              /* 0x00 */
    unsigned int     flags;                                 /* 0x20 */
    unsigned int     unused[2];                             /* 0x24 */
    __int16          manual_structure_bsp_reference_index;  /* 0x2C */
    __int16          runtime_structure_bsp_reference_index; /* 0x2E */
    tag_block        commands;                              /* 0x30 (ai_command_definition[]) */
    tag_block        points;                                /* 0x3C (ai_command_point_definition[]) */
    tag_block        unused_blocks[2];                      /* 0x48 */
} ai_command_list_definition;                               /* 0x60 = 96 bytes */
