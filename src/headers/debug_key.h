#pragma once
#include <stdint.h>
/* debug_key — one entry in the static debug-key binding table (debug_keys.c) */
typedef struct debug_key
{
    const char    *name;             /* 0x00 */
    int16_t        key_code;         /* 0x04 — scancode */
    int16_t        modifier;         /* 0x06 — required modifier combo (0..3) */
    void         (*function)(unsigned char pressed);  /* 0x08 — edge callback */
    unsigned char  toggle_variable;  /* 0x0C — if set, `variable` is toggled on release */
    unsigned char _pad0[3]; /* db-verified padding */
    unsigned char *variable;         /* 0x10 — bound bool */
} debug_key;                         /* 20 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern debug_key global_debug_key_list[];
extern int *global_debug_key_down;   /* one "is-down" bit per key */

#ifdef __cplusplus
}
#endif
