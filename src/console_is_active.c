/* console_is_active @ 0x83721F28 */
#include <stdint.h>
#include "headers/console_globals.h"
/* return attested uint8_t: lbz load + every caller re-normalizes with clrlwi r,r3,24 (6/6) */
uint8_t console_is_active(void)
{
    return console_globals.active;
}
