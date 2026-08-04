#pragma once

/* hs_source_file — an entry in the scenario's source_files tag_block (52 bytes). DB-verified
 * layout (types_members hs_source_file): a 32-char name followed by the tag_data holding the
 * HaloScript source text. Used by hs_rebuild_source_file / hs_scenario_merge. */

#include "tag_data.h"

typedef struct hs_source_file
{
    char     name[32];   /* 0x00 */
    tag_data source;     /* 0x20 (32) — the script source text blob */
} hs_source_file;
