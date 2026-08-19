/* main_load_core @0x83689260 — request that the engine core be reloaded next frame. */

#include "headers/main_globals.h"

void main_load_core(void)
{
    main_globals.load_core = 1;
}
