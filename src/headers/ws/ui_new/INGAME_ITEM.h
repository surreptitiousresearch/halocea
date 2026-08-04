#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine ui_new: one queued "ingame" debug-text call (world/screen-anchored text drawn once
// this frame, e.g. via a debug-draw API) — a screen position plus its ASCII source text.
// DB-verified layout (types_members ui_new::INGAME_ITEM): x@0, y@4, StatData@8
// (dsTSTRING<char>) — size 12.

namespace ui_new {

typedef struct INGAME_ITEM {
    int             x;        // 0x00
    int             y;        // 0x04
    dsTSTRING<char> StatData; // 0x08 ASCII text — converted to wide via gsSTRINGS::AsciiToUnicode
                                //      before being printed (see UI_STAT_SYS::RenderIngameStat)
} INGAME_ITEM;

} // namespace ui_new
