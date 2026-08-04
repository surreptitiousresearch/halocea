/* immediate_transparent_geometry_group @ 0x8446C208 — the singleton transparent_geometry_group used by
 * _rasterizer_model_transparent_geometry_submit for the immediate/no-queue (decal / active-camouflage)
 * draw path. data-bss zero-fill, DB type transparent_geometry_group (168 bytes).
 * P4 sweep flagged accesses at +0xA8 (== struct size) through +0x1B8: these fall in the 512-byte gap to
 * local_parameters (0x8446C408) and are SEPARATE rasterizer local_* globals (DB dword_8446C2B0,
 * unk_8446C3A8, unk_8446C3C0) reached off the local_parameters base register (r30=0x8446C418), not off
 * this struct's base. Sibling-block access, not overflow. Type transparent_geometry_group is correct. */
#include "../headers/transparent_geometry_group.h"

transparent_geometry_group immediate_transparent_geometry_group;
