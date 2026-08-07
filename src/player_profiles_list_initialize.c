/* player_profiles_list_initialize @0x83692A48 — populate a UI list widget with every player profile
 * available to its owning local player. Resets the shared profile-delete/cache state, (re)allocates the
 * widget's list_items buffer (100 entries), and enumerates available profiles into it. If the widget's own
 * tag definition declares exactly 3 child widgets (fixed 3-slot layout), pads any missing slots up to 3
 * with -1.
 * Builds the preview list bank from each resolved profile, tracking (and pre-selecting) player 1's
 * last-used profile if present; defaults the selection to the first entry otherwise. */

#include <stdint.h>
#include <string.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/global_tag_instances.h"
#include "headers/cached_player_profile.h"
#include "headers/ui_widget_definition.h"
#include "headers/blam_data_globals.h"


extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void player_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);
extern void preview_list_create(void);
extern int player_ui_get_player1_last_used_profile_index(void);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern void preview_list_add_item_to_bank(int bank, const wchar_t *name, int list_index, void *data,
        unsigned int data_len, char is_default);

uint8_t player_profiles_list_initialize(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    ui_widget_definition *widget_definition =
        TAG_GET(ui_widget_definition, list_widget->ui_widget_tag_index);

    delete_profile_index = -1;
    memset(cached_player_profile, -1, sizeof(cached_player_profile));

    void *list_items_buffer = ui_widget_realloc(list_widget->parameters.list_parameters.list_items, 0x190u);
    list_widget->parameters.list_parameters.list_items = list_items_buffer;

    if ( list_items_buffer )
    {
        int *list_items = (int *)list_items_buffer;
        uint16_t number_of_profiles = 100;
        player_profiles_enumerate_available_to_local_player_index(list_widget->local_player_index,
                &number_of_profiles, list_items, 0);

        if ( widget_definition->child_widget_references.count == 3 && number_of_profiles < 3 )
        {
            for ( uint16_t i = number_of_profiles; i < 3; ++i )
                list_items[i] = -1;
            number_of_profiles = 3;
        }

        list_widget->parameters.list_parameters.number_of_items = number_of_profiles;
        preview_list_create();

        int last_used_profile_index = player_ui_get_player1_last_used_profile_index();
        list_widget->parameters.text_box_parameters.string_list_index = -1;

        for ( int16_t i = 0; i < list_widget->parameters.list_parameters.number_of_items; ++i )
        {
            if ( last_used_profile_index != -1 && list_items[i] == last_used_profile_index )
                list_widget->parameters.text_box_parameters.string_list_index = i;

            player_profile profile;
            if ( player_profile_get(list_items[i], &profile) )
            {
                preview_list_add_item_to_bank(0, profile.player_name, i, &profile, 1980,
                        list_widget->parameters.text_box_parameters.string_list_index == i);
            }
        }

        if ( (uint16_t)list_widget->parameters.text_box_parameters.string_list_index == 0xFFFF )
            list_widget->parameters.text_box_parameters.string_list_index = 0;

        list_widget->parameters.list_parameters.list_item_top_index = -1;
        list_widget->parameters.list_parameters.selected_list_item_index =
                list_widget->parameters.text_box_parameters.string_list_index;
    }

    return 1;
}
