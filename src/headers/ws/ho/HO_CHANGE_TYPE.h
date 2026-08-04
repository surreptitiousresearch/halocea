#pragma once
// ws-engine ho: hideout-change transition kind. DB-verified (types_enum_values HO_CHANGE_TYPE).
enum HO_CHANGE_TYPE {
    HO_CHANGE_LEFT_DOWN      = 0,
    HO_CHANGE_RIGHT_DOWN     = 1,
    HO_CHANGE_LEFT_FORW      = 2,
    HO_CHANGE_RIGHT_FORW     = 3,
    HO_CHANGE_BACKFLIP       = 4,
    HO_CHANGE_UP_LONG        = 5,
    HO_CHANGE_FULL           = 6,
    HO_CHANGE_STAY_LEFT_OUT  = 7,
    HO_CHANGE_STAY_RIGHT_OUT = 8,
};
