#pragma once
// ws-engine fsm: the empty tag base of every FSM callback parameter block.
// DB `types fsmCB_PARAM_BASE` = struct, size 1; `types_members fsmCB_PARAM_BASE` returns zero
// rows — it carries no data. It appears as an is_baseclass row (offset 0, size 0) on
// fsmWPN_LIST_PARAM_WEAPON, and as the second empty base of fsmPARAM_CDT (there at offset 1,
// behind the DB's `gap0` byte, because a second empty base needs its own address).
struct fsmCB_PARAM_BASE {};
