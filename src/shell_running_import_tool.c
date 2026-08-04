/* shell_running_import_tool @0x836FBD40 — true when running inside the asset import tool. Always false in the
 * shipped runtime. */

#include <stdint.h>

/* attested uint8_t: all 4 callers clrlwi r3,24 after the call (Blam boolean) */
uint8_t shell_running_import_tool(void)
{
    return 0;
}
