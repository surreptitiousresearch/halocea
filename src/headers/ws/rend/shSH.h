#pragma once
// ws-engine spherical-harmonics lighting coefficient set (no dedicated "sh" subsystem folder
// exists in this corpus yet; kept alongside rend since rendLIGHT_SET is its only consumer here).
// DB-verified layout (types_members shSH): coeffs@0 (float[9]) — size 36.

typedef struct shSH {
    float coeffs[9]; // 0x00

    // Default-construct a zeroed SH coefficient set. Body not decompiled here; used by
    // rendLIGHT_SET's constructor to zero-init desiredStaticLightState/curStaticLightState's
    // shColor.shR/shG/shB before the surrounding memset also zeroes them.           boundary
    shSH(); // boundary
} shSH;
