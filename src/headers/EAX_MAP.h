#pragma once
/* EAX_MAP — the static table in sound_dsound_fmod.cpp mapping a Halo sound_environment tag name to the
 * FMOD reverb-preset name to apply. 23 entries, 8 bytes each: a tag-name C string pointer at +0 and the
 * preset name as a dsTSTRING<char> at +4 (element stride 8, confirmed by disasm slwi index,3 / addi 8).
 * The table's data lives in the sound_dsound_fmod translation unit; declared extern here. */

#include "ws/ds/dsTSTRING.h"

typedef struct EAX_MAP_ENTRY
{
    const char      *tag_name;     /* 0x00 */
    dsTSTRING<char>  fmod_preset;  /* 0x04 */
} EAX_MAP_ENTRY;                   /* 8 bytes */

extern EAX_MAP_ENTRY EAX_MAP[23];
