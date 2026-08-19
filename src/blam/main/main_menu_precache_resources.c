/* main_menu_precache_resources @0x83689458 — precache the predicted UI resources of the loaded scenario. */

#include "headers/scenario.h"

#include "headers/tag_block.h"
extern void predicted_resources_precache(const tag_block *predicted_resources);

void main_menu_precache_resources(void)
{
    if ( global_scenario )
        predicted_resources_precache(&global_scenario->predicted_ui_resources);
}
