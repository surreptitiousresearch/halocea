/* main_set_timeout @0x83689010 — set the idle timeout (in milliseconds) used by the demo-attract logic. */

#include "headers/main_globals.h"

void main_set_timeout(int msec)
{
    main_globals.idle_timeout = msec;
}
