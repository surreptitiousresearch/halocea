#pragma once
// ws-engine ia sound subsystem: how a sound is silenced.
// DB-verified values (types_enum_values iaSND_MUTE_TYPE).

enum iaSND_MUTE_TYPE {
    SMT_NORMAL         = 0, // fade/stop per the sound's normal release
    SMT_NOW            = 1, // stop immediately
    SMT_NOW_NONLOOPED  = 2, // stop immediately, but let non-looped sounds finish
};
