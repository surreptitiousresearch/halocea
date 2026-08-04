/* main_taking_screenshot @0x8368AA08 — return whether a screenshot or movie capture is currently in flight. */

#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"


int main_taking_screenshot(void)
{
    if ( global_screenshot_count > 0 )
        return 1;
    return main_globals.movie != 0;
}
