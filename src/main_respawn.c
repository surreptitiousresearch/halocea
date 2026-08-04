/* main_respawn @0x83689218 — request the player respawn; a forced respawn arms the 91-tick respawn timer. */

#include <stdint.h>
#include "headers/main_globals.h"

/* param uint8_t: r3 first read via clrlwi r9,r3,24 (8-bit) at 8368921C. */
void main_respawn(uint8_t force_respawn)
{
    main_globals.respawn = 1;
    if ( force_respawn )
        main_globals.respawn_timer = 91;
}
