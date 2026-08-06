/* hs_type_names @ 0x84177F68 (.data, 196 bytes)
 * DB applied_types: const char *hs_type_names[49];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82118ED0 -> "unparsed"
 *   +0x0004 = 0x82118EC0 -> "special form"
 *   +0x0008 = 0x82118EB0 -> "function name"
 *   +0x000C = 0x82118EA4 -> "passthrough"
 *   +0x0010 = 0x8202794C -> "void"
 *   +0x0014 = 0x820F9304 -> "boolean"
 *   +0x0018 = 0x82118E9C -> "real"
 *   +0x001C = 0x820253E8 -> "short"
 *   +0x0020 = 0x82118E94 -> "long"
 *   +0x0024 = 0x82025880 -> "string"
 *   +0x0028 = 0x82026354 -> "script"
 *   +0x002C = 0x82118E84 -> "trigger_volume"
 *   +0x0030 = 0x82118E74 -> "cutscene_flag"
 *   +0x0034 = 0x82118E5C -> "cutscene_camera_point"
 *   +0x0038 = 0x82118E4C -> "cutscene_title"
 *   +0x003C = 0x82118E38 -> "cutscene_recording"
 *   +0x0040 = 0x82118E28 -> "device_group"
 *   +0x0044 = 0x82060C48 -> "ai"
 *   +0x0048 = 0x82118E18 -> "ai_command_list"
 *   +0x004C = 0x82118E04 -> "starting_profile"
 *   +0x0050 = 0x82118DF4 -> "conversation"
 *   +0x0054 = 0x82118DE8 -> "navpoint"
 *   +0x0058 = 0x82118DDC -> "hud_message"
 *   +0x005C = 0x82118DD0 -> "object_list"
 *   +0x0060 = 0x82026334 -> "sound"
 *   +0x0064 = 0x8207EDC4 -> "effect"
 *   +0x0068 = 0x820745E4 -> "damage"
 *   +0x006C = 0x82118DC0 -> "looping_sound"
 *   +0x0070 = 0x82118DB0 -> "animation_graph"
 *   +0x0074 = 0x82118DA0 -> "actor_variant"
 *   +0x0078 = 0x82118D90 -> "damage_effect"
 *   +0x007C = 0x82118D7C -> "object_definition"
 *   +0x0080 = 0x82118D6C -> "game_difficulty"
 *   +0x0084 = 0x820292EC -> "team"
 *   +0x0088 = 0x82118D58 -> "ai_default_state"
 *   +0x008C = 0x82118D4C -> "actor_type"
 *   +0x0090 = 0x82118D40 -> "hud_corner"
 *   +0x0094 = 0x82026324 -> "object"
 *   +0x0098 = 0x8211463C -> "unit"
 *   +0x009C = 0x8211462C -> "vehicle"
 *   ... 36 further bytes elided; full hex in .sweep/data_image.tsv
 * data-init const char *[49]. HaloScript value/type names,
 * indexed by hs_type enum. Pointers recovered from the binary (big-endian); string targets
 * read from .rdata.
 */
const char *hs_type_names[49] =
{
    "unparsed",              /*  0 */
    "special form",          /*  1 */
    "function name",         /*  2 */
    "passthrough",           /*  3 */
    "void",                  /*  4 */
    "boolean",               /*  5 */
    "real",                  /*  6 */
    "short",                 /*  7 */
    "long",                  /*  8 */
    "string",                /*  9 */
    "script",                /* 10 */
    "trigger_volume",        /* 11 */
    "cutscene_flag",         /* 12 */
    "cutscene_camera_point", /* 13 */
    "cutscene_title",        /* 14 */
    "cutscene_recording",    /* 15 */
    "device_group",          /* 16 */
    "ai",                    /* 17 */
    "ai_command_list",       /* 18 */
    "starting_profile",      /* 19 */
    "conversation",          /* 20 */
    "navpoint",              /* 21 */
    "hud_message",           /* 22 */
    "object_list",           /* 23 */
    "sound",                 /* 24 */
    "effect",                /* 25 */
    "damage",                /* 26 */
    "looping_sound",         /* 27 */
    "animation_graph",       /* 28 */
    "actor_variant",         /* 29 */
    "damage_effect",         /* 30 */
    "object_definition",     /* 31 */
    "game_difficulty",       /* 32 */
    "team",                  /* 33 */
    "ai_default_state",      /* 34 */
    "actor_type",            /* 35 */
    "hud_corner",            /* 36 */
    "object",                /* 37 */
    "unit",                  /* 38 */
    "vehicle",               /* 39 */
    "weapon",                /* 40 */
    "device",                /* 41 */
    "scenery",               /* 42 */
    "object_name",           /* 43 */
    "unit_name",             /* 44 */
    "vehicle_name",          /* 45 */
    "weapon_name",           /* 46 */
    "device_name",           /* 47 */
    "scenery_name",          /* 48 */
};
