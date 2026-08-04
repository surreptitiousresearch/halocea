#include <stdint.h>
#include "headers/blam_data_globals.h"
/* credits_load_directly_for_endgame @0x83697EA0 — load the credits screen widget directly (parented to the
 * main-menu tag) and switch to end-game credits mode. */

typedef struct widget_instance widget_instance;

extern int tag_loaded(uint32_t group_tag, const char *name);
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);

widget_instance *credits_load_directly_for_endgame(void)
{
    int main_menu_tag = tag_loaded(0x44654C61u /* 'DeLa' */, "ui\\shell\\main_menu\\main_menu");
    widget_instance *result =
        ui_widget_load_by_name_or_tag("ui\\shell\\main_menu\\credits_screen", -1, 0, -1, main_menu_tag, -1, -1);
    credits_endgame_mode = 1;
    return result;
}
