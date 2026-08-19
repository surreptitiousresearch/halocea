/* main_run_demos_private @0x8368A748 — clear the run-xdemos request and launch the Xbox demo loop.
 *
 * Deviation: database types this int; xbox_demos_launch is void no-arg, so the return is residue. */

#include "headers/main_globals.h"

extern void xbox_demos_launch(void);

void main_run_demos_private(void)
{
    main_globals.run_xdemos = 0;
    xbox_demos_launch();
}
