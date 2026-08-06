#pragma once
#include "../ds/dsTSTRING.h"
#include "UI_STAT_PARAMS.h"
// ws-engine ui_new: one registered "UI stat" line (top-of-screen debug counter row) — a pair of
// caller-supplied callbacks that (re)fill its display params/text each frame, plus the
// already-formatted wide text to draw.
// DB-verified layout (types_members ui_new::UI_STAT_ITEM): GetStatParamsCB@0, GetStatDataCB@4,
// StatData@8 (dsTSTRING<wchar_t>) — size 12.

namespace ui_new {

typedef struct UI_STAT_ITEM {
    void (*GetStatParamsCB)(UI_STAT_PARAMS *outParams); // 0x00
    void (*GetStatDataCB)(dsTSTRING<wchar_t> *outText); // 0x04
    dsTSTRING<wchar_t>  StatData;                                   // 0x08
} UI_STAT_ITEM;

} // namespace ui_new
