/* virtual_keyboard_close @0x83783ED0 — tail-call forwarder (`b virtual_keyboard_cancel`; confirmed via
 * disasm, not a bare blr). "Close" collapsed to "cancel" in the retail build. virtual_keyboard_cancel
 * itself is not yet decompiled — declared extern with its own DB prototype and called through. */

#include <stdint.h>

extern uint8_t virtual_keyboard_cancel(void);

uint8_t virtual_keyboard_close(void)
{
    return virtual_keyboard_cancel();
}
