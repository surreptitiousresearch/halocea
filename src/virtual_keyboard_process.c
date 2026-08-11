/* virtual_keyboard_process @0x83784780 — per-frame tick of the on-screen keyboard; runs the internal
 * processor only while the keyboard is active.
 *
 * DEVIATION (2026-08-11): this was declared `int` returning an uninitialized local, on the note that the
 * sole caller forwards r3 into event_manager_flush's controller_index. It does not. The binary is
 *   0x8378478C  beqlr                                (returns with r3 untouched)
 *   0x83784790  b     virtual_keyboard_process_internal   (tail call into a void callee)
 * so no path here ever defines r3, and the caller's next instruction is a bare `bl event_manager_flush`
 * (process_ui_widgets 0x83737128) with no argument setup — event_manager_flush @0x837263F8 is void(void)
 * and its first act is `addi r3, r11, 0xC` (its own memset Dst). The value was never read; the function is
 * void. process_ui_widgets.c had already recorded this correctly ("phantom arg removed"). */

extern void virtual_keyboard_process_internal(void);

#include "headers/virtual_keyboard_globals_t.h"

void virtual_keyboard_process(void)
{
    if ( virtual_keyboard_globals.active )
        virtual_keyboard_process_internal();
}
