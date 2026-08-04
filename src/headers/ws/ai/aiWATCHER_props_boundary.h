#pragma once
#include "../ctrl/ctrlPROP_ID.h"
// ctrl-subsystem model/state property-id globals (const ctrlPROP_ID) read by
// aiWATCHER::TranslateSTRID_FLAGS when packing the body's property bit-cache into the watcher's
// AI_FSM_PROPS flag word. All are `?..@@3UctrlPROP_ID@@B` (const) in the binary. boundary data.

extern const ctrlPROP_ID MDL_ST_PROP_MINIZOOM;
extern const ctrlPROP_ID MDL_ST_PROP_REACHED_MZ;
extern const ctrlPROP_ID MDL_ST_PROP_WPN_SHOOTING;
extern const ctrlPROP_ID MDL_ST_PROP_FLOOR_HO_MINIZOOM;
extern const ctrlPROP_ID MDL_ST_PROP_WLN_HO_MINIZOOM;
extern const ctrlPROP_ID MDL_ST_PROP_UNCONTROLLED;
extern const ctrlPROP_ID MDL_ST_PROP_FALLING;
extern const ctrlPROP_ID MDL_ST_PROP_HIDEOUT;
extern const ctrlPROP_ID MDL_ST_PROP_REACHED_COVER;
extern const ctrlPROP_ID MDL_ST_PROP_HIDEOUT_LEAVE; // ?MDL_ST_PROP_HIDEOUT_LEAVE@@3UctrlPROP_ID@@B
extern const ctrlPROP_ID MDL_ST_PROP_SMART_OBJ;
extern const ctrlPROP_ID HO_POS_LOWHO;
extern const ctrlPROP_ID HO_POS_LEFT;
extern const ctrlPROP_ID HO_POS_RIGHT;
extern const ctrlPROP_ID HO_POS_OUTER;
extern const ctrlPROP_ID MDL_ST_PROP_UNCONSCIOUS;
extern const ctrlPROP_ID MDL_ST_PROP_LOW_G;
extern const ctrlPROP_ID MDL_ST_PROP_WLN;
extern const ctrlPROP_ID MDL_PROP_ST_WLN_JUMP_FLY;
extern const ctrlPROP_ID MDL_ST_FLAG_IS_PROC_AIM_NOT_CLAMPED; // ?..@@3UctrlPROP_ID@@B
extern const ctrlPROP_ID MDL_ST_PROP_SHOOTING_ANIM;
extern const ctrlPROP_ID MDL_ST_PROP_DISABLED_SHOOTING_ANIM;
