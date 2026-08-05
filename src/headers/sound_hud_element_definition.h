#pragma once
#include <stdint.h>
#include "tag_reference.h"
/* sound_hud_element_definition — one entry of a HUD element's sound block (56 bytes). `sound` is a
 * tag reference whose group tag is 'lsnd' for a looping sound and 'snd!' for an impulse one, which
 * is how hud_play_sound decides between starting a loop and retriggering on a rising edge.
 * `type_flags` is the condition mask tested against the caller's active-condition set.
 * Layout verbatim from the database (types_members sound_hud_element_definition). */

typedef struct sound_hud_element_definition
{
    tag_reference sound;      /* 0x00 group_tag@0x00, index@0x0C */
    int           type_flags; /* 0x10 condition mask */
    float         scale;      /* 0x14 */
    int           unused[3];  /* 0x18 */
    uint8_t       pad0;       /* 0x24 */
    uint8_t       pad[3];     /* 0x25 */
    int           unused2[4]; /* 0x28 */
} sound_hud_element_definition; /* 0x38 */
