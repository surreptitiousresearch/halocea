/* shell_get_clipboard_text @ 0x836FBD90 — clipboard access is unavailable in this build. */

#include <stdint.h>

/* Return is a Blam boolean: the sole caller (console_clipboard_paste @83721EEC) byte-normalizes r3
 * with clrlwi 24 => declared uint8_t. Params proven at the call site (r3=buffer, r4=length 0xFF);
 * this stub ignores them. */
uint8_t shell_get_clipboard_text(char *buffer, int length)
{
    return 0;
}
