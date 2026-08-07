#pragma once
// ws-engine ent: entENTITY state flag bits. DB-verified (types_enum_values ENT_ST); size 4.
// ENT_ST_LASTSTATE is the binary's own sentinel (highest bit | 1), transcribed verbatim.
enum ENT_ST {
    ENT_ST_RUNTIME             = 0x1,
    ENT_ST_PERMANENT           = 0x2,
    ENT_ST_KEEP_RELOAD         = 0x4,
    ENT_ST_FARM_DESTROY        = 0x80,
    ENT_ST_FARMED              = 0x100,
    ENT_ST_FARM_TERMINATE      = 0x200,
    ENT_ST_SHOOTABLE_BY_AI     = 0x400,
    ENT_ST_AIMABLE_BY_AI       = 0x800,
    ENT_ST_HIDDEN              = 0x1000,
    ENT_ST_HAS_PROP_GRAVITY    = 0x2000,
    ENT_ST_SPEC_PRIOR          = 0x4000,
    ENT_ST_END_EFFECT          = 0x8000,
    ENT_ST_IS_AFFECT_DOM_EVENT = 0x10000,
    ENT_ST_WATER               = 0x20000,
    ENT_ST_WANT_FF_NOTIFY      = 0x80000,
    ENT_ST_SET_START_ON_SHOW   = 0x2000000,
    ENT_ST_IS_SUBSYSTEM        = 0x4000000,
    ENT_ST_LASTSTATE           = 0x4000001,
};
