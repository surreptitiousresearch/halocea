#pragma once
// ws-engine ia: network-sync ownership mode for an actor descriptor's default state.
// DB-verified values (types_enum_values iaSYNC_MODE): LOCAL=0, HOST=1, SYNC=2, LOCAL_PLAYER=3.

enum iaSYNC_MODE {
    iaSYNC_LOCAL        = 0,
    iaSYNC_HOST         = 1,
    iaSYNC_SYNC         = 2,
    iaSYNC_LOCAL_PLAYER = 3,
};
