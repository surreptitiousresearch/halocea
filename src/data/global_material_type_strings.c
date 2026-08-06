/* global_material_type_strings @ 0x84184AC0 (.data, 132 bytes)
 * DB applied_types: char *global_material_type_strings[33];
 * DEVIATION: was authored with an unbounded `[]` and a 34th element of 0, on the strength of
 * the distance to the next named symbol. That distance is an UPPER BOUND, not a size (the
 * same measurement that made sound_is_queueable [48] when it is [44]). Three oracles say 33:
 * applied_types declares [33], the image extent is 132 = 33*4, and material_type.h states
 * NUMBER_OF_MATERIAL_TYPES = 33, DB-verified via types_enum_values. The bound is now that
 * enum, so the two cannot drift apart again.
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82002A4C -> "dirt"
 *   +0x0004 = 0x82002A44 -> "sand"
 *   +0x0008 = 0x82002A3C -> "stone"
 *   +0x000C = 0x82002A34 -> "snow"
 *   +0x0010 = 0x82002A2C -> "wood"
 *   +0x0014 = 0x8212624C -> "metal (hollow)"
 *   +0x0018 = 0x8212623C -> "metal (thin)"
 *   +0x001C = 0x8212622C -> "metal (thick)"
 *   +0x0020 = 0x820029FC -> "rubber"
 *   +0x0024 = 0x820029F4 -> "glass"
 *   +0x0028 = 0x82126220 -> "force field"
 *   +0x002C = 0x820029DC -> "grunt"
 *   +0x0030 = 0x82126210 -> "hunter armor"
 *   +0x0034 = 0x82126204 -> "hunter skin"
 *   +0x0038 = 0x820029B8 -> "elite"
 *   +0x003C = 0x820029B0 -> "jackal"
 *   +0x0040 = 0x821261EC -> "jackal energy shield"
 *   +0x0044 = 0x821261DC -> "engineer skin"
 *   +0x0048 = 0x821261C4 -> "engineer force field"
 *   +0x004C = 0x821261B0 -> "flood combat form"
 *   +0x0050 = 0x8212619C -> "flood carrier form"
 *   +0x0054 = 0x8212618C -> "cyborg armor"
 *   +0x0058 = 0x82126174 -> "cyborg energy shield"
 *   +0x005C = 0x82126168 -> "human armor"
 *   +0x0060 = 0x8212615C -> "human skin"
 *   +0x0064 = 0x82002928 -> "sentinel"
 *   +0x0068 = 0x82002920 -> "monitor"
 *   +0x006C = 0x82002918 -> "plastic"
 *   +0x0070 = 0x82002910 -> "water"
 *   +0x0074 = 0x82002908 -> "leaves"
 *   +0x0078 = 0x82126148 -> "elite energy shield"
 *   +0x007C = 0x820028F4 -> "ice"
 *   +0x0080 = 0x82126138 -> "hunter shield"
 * Blam .lib data-global storage definition (init batch 7).
 */
/* global_material_type_strings @0x84184AC0, DB extent 136 = const char *[34]
 * (33 names + trailing NULL). */
#include "../headers/material_type.h"

const char *global_material_type_strings[NUMBER_OF_MATERIAL_TYPES] = {
    "dirt","sand","stone","snow","wood","metal (hollow)","metal (thin)",
    "metal (thick)","rubber","glass","force field","grunt","hunter armor",
    "hunter skin","elite","jackal","jackal energy shield","engineer skin",
    "engineer force field","flood combat form","flood carrier form",
    "cyborg armor","cyborg energy shield","human armor","human skin",
    "sentinel","monitor","plastic","water","leaves","elite energy shield",
    "ice","hunter shield"
};
