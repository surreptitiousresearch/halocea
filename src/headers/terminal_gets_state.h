#pragma once
/* terminal_gets_state — an active console gets() input session: the pending keystrokes, the prompt and
 * accumulated result text, and the inline edit-text cursor state (DB layout, 448 bytes). */

#include <stdint.h>
#include "key_stroke.h"
#include "real_argb_color.h"
#include "edit_text.h"

typedef struct terminal_gets_state
{
    int16_t         key_count;   /* 0x000 */
    key_stroke      keys[32];    /* 0x002 */
    char            _pad82[2];   /* 0x082 */
    real_argb_color color;       /* 0x084 */
    char            prompt[32];  /* 0x094 */
    char            result[256]; /* 0x0B4 */
    edit_text       edit;        /* 0x1B4 */
} terminal_gets_state;           /* 448 bytes */
