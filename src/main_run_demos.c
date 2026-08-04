/* main_run_demos @0x83689718 — request that the Xbox demo loop be launched. */

#include "headers/main_globals.h"

void main_run_demos(void)
{
    main_globals.run_xdemos = 1;
}
