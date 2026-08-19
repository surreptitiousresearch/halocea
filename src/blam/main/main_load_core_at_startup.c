/* main_load_core_at_startup @0x83689278 — request that the engine core be loaded once at startup. */

#include "headers/main_globals.h"

void main_load_core_at_startup(void)
{
    main_globals.load_core_at_startup = 1;
}
