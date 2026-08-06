#pragma once
/* ai_command_list_definition — one scenario AI "command list" (scripted order sequence) tag block
 * element (96 bytes). Layout transcribed verbatim from the database (types_members).
 * NAME COLLISION: the binary carries BOTH a 96-byte type named ai_command_list_definition and a
 * global of that name (the hs_function_table entry @0x8211A90C, type hs_function_definition —
 * blam_data_globals.h). C keeps typedef names and object names in one namespace, so the DB name is
 * preserved on the struct tag and the typedef takes the `_t` suffix. */

#include <stdint.h>
#include "tag_block.h"

typedef struct ai_command_list_definition
{
    char             name[32];                              /* 0x00 */
    unsigned int     flags;                                 /* 0x20 */
    unsigned int     unused[2];                             /* 0x24 */
    int16_t          manual_structure_bsp_reference_index;  /* 0x2C */
    int16_t          runtime_structure_bsp_reference_index; /* 0x2E */
    tag_block        commands;                              /* 0x30 (ai_command_definition[]) */
    tag_block        points;                                /* 0x3C (ai_command_point_definition[]) */
    tag_block        unused_blocks[2];                      /* 0x48 */
} ai_command_list_definition_t;                             /* 0x60 = 96 bytes */
