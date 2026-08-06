#pragma once
/* console_globals @ console_globals — in-game console / terminal input state (console.c) */

#include <stdint.h>
#include "key_stroke.h"
#include "edit_text.h"
#include "terminal_gets_state.h"
#include "real_argb_color.h"

/* DB anonymous struct: types_members _B58B0D509AE75C5D1BF5B80E668D705B */
typedef struct console_globals_t
{
    unsigned char       active;                          /* 0x000 */
    unsigned char       available;                       /* 0x001 */
    char                _pad02[2];                       /* 0x002 */
    terminal_gets_state input_state;                     /* 0x004 */
    char                previous_commands[8][255];       /* 0x1C4 */
    int16_t             previous_command_count;          /* 0x9BC */
    int16_t             newest_previous_command_index;   /* 0x9BE */
    int16_t             selected_previous_command_index; /* 0x9C0 */
} console_globals_t;

#ifdef __cplusplus
extern "C" {
#endif

extern console_globals_t console_globals;
extern real_argb_color console_color;

#ifdef __cplusplus
}
#endif
