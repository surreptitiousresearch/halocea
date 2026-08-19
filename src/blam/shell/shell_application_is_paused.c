/* shell_application_is_paused @ 0x836FBD48 — never paused on this build */
#include <stdint.h>

/* attested uint8_t: all 3 callers clrlwi r3,24 after the call (Blam boolean) */
uint8_t shell_application_is_paused(void)
{
    return 0;
}
