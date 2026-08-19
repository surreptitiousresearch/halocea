/* warn_if_difficulty_will_nuke_saved_game @0x83780178 — hides the difficulty screen's warning sub-widget
 * (confirmed via decompiler: `difficulty_screen_widget->children->next->visible = 0;`). */

#include "headers/widget_instance.h"

void warn_if_difficulty_will_nuke_saved_game(widget_instance *difficulty_screen_widget)
{
    difficulty_screen_widget->children->next->visible = 0;
}
