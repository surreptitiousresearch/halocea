#pragma once
#include "../ia/iaIACTOR.h"
// ws-engine gs: level timer actor — an iaIACTOR-derived object ticked by the message system
// that tracks the current level-relative time. Treated as an OTHER-SUBSYSTEM BOUNDARY here;
// only the one field callers read (timeLife) is modeled.
// DB-verified layout (types_members gsLVL_TIMER): <iaIACTOR base>@0 (200), timeLife@200 — size 204.

struct gsLVL_TIMER : iaIACTOR {
    float timeLife; // 0xC8 current level-relative time (seconds)
};

// ?gsLvlTimer@@3PAVgsLVL_TIMER@@A — the singleton level timer.
extern gsLVL_TIMER *gsLvlTimer;
