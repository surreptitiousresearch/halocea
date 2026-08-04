#pragma once
// ws-engine ai08: approach-module state bitmask (aiAPPROACH::state). DB-verified values
// (types_enum_values AI_APST). Held in a dsFLAGS<AI_APST,int>.
enum AI_APST {
    AI_APST_APPROACH_ENABLED = 0x1,    // module active this frame
    AI_APST_SOMEONE_IN_ZONE  = 0x10,   // last CheckDistance found a watched entity inside `radius`
    AI_APST_ACT_ONCE         = 0x100,  // fire the enter event only once ("isActOnce")
    AI_APST_ACTED            = 0x200,  // (reserved; cleared by ParsePS)
    AI_APST_NOTICE_PLAYER1   = 0x1000, // watch player 0
    AI_APST_NOTICE_PLAYER2   = 0x2000, // watch remaining players
    AI_APST_NOTICE_ALLY      = 0x3000, // watch the on-screen ally (both notice bits)
    AI_APST_LASTSTATE        = 0x3001,
};
