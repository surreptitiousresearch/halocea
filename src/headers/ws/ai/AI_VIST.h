#pragma once
// ws-engine ai08: aiVISION_COMMON::state bit flags. DB-verified (headers_ref AI_VIST).

enum AI_VIST {
    AI_VIST_SCR_EAGLE_EYE     = 0x2, // scripted eagle-eye (ignore cone, distance only)
    AI_VIST_HAS_FRESH_RESULTS = 0x4,
    AI_VIST_GRABBED_CHEAP     = 0x8,
    AI_VIST_LASTSTATE         = 0x9,
};
