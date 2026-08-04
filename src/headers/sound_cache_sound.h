#pragma once
/* sound_cache_sound — one 16-byte record in pc_sound_cache_globals.cache_sounds (the LRUV cache-block
 * bookkeeping datum for a resident sound permutation). Layout reconstructed from the raw-offset accesses
 * in _sound_cache_sound_request (no DB struct type exists for this record); fields whose meaning is not
 * certain are marked. Element stride is 16 bytes (index shifted by 4 in the code). */

struct sound_permutation;

typedef struct sound_cache_sound
{
    __int16                   identifier;       /* 0x00 — datum id (uncertain) */
    unsigned __int8           allocated;        /* 0x02 — record allocated/ready (set 1 on new) */
    unsigned __int8           loaded;           /* 0x03 — data present / preload complete */
    unsigned __int8           _unknown04;       /* 0x04 — uncertain */
    unsigned __int8           reference_count;  /* 0x05 — outstanding references */
    unsigned __int8           _unknown06;       /* 0x06 — cleared on load (uncertain) */
    unsigned __int8           _unknown07;       /* 0x07 — uncertain */
    __int16                   _unused08;        /* 0x08 — cleared on new */
    __int16                   _unknown0A;       /* 0x0A — uncertain */
    struct sound_permutation *sound;            /* 0x0C — owning permutation */
} sound_cache_sound;                            /* 16 bytes */
