#pragma once
// ws-engine fsm: the empty tag base every FSM event family derives from.
// DB `types fsmEVENT_BASE` = struct, size 1; `types_members fsmEVENT_BASE` returns zero rows —
// it carries no data. `types_members` records it as member_index 0, is_baseclass=1, offset 0,
// size 0 of fsmAIMING_EVENT / fsmBODY_FSM_EVENT / fsmFSM_EVENT / fsmHIDEOUT_EVENT /
// fsmMOVE_EVENT / fsmWEAPON_EVENT, each of which the DB sizes at 1 byte — i.e. the base is
// empty-base-optimized away and deriving it is layout-neutral.
struct fsmEVENT_BASE {};
