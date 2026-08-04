/* server_list_menu_update @0x8377E6A0 — per-frame refresh of the legacy system-link/LAN "server browser"
 * list widget and its attached "selected server" detail panel (icons, map/gametype/team/player-count/
 * score-to-win text, and a "searching..." / "no games found" info panel).
 *
 * VERIFIED DEAD IN THIS BINARY (disasm 0x8377E714-0x8377E724, cross-checked against
 * network_game_client_get_available_games.c / network_game_client_advertised_game_is_valid.c, both retail
 * stubs that unconditionally return 0/false): the population loop initializes its walking pointer to the
 * exact same value as its break-bound (`v7 = displayed_servers; if (v7 >= displayed_servers) break;`), so it
 * never executes a single iteration, and `displayed_servers` is never populated by anything else (the game's
 * "get available games" call always returns NULL) — the array is always all-zero, freshly re-zeroed by this
 * function's own opening loop. Likewise `widget->parameters.text_box_parameters.string_list_index` (the
 * "selected server" index) is unconditionally clamped to <= -1 by this very function before it is read, so
 * the `selected_index >= 0` detail-panel branch below is also unreachable — every call falls through to the
 * "no server selected" default state at the bottom (info_widget shown, its text picked by whether >= 1 second
 * has elapsed since the widget was created). System-link game discovery is compiled out; this function (and
 * advertised_game_data's field layout) is retained faithfully because that is what the binary contains, not
 * because the branches run.
 *
 * DEVIATION: `*(_WORD *)(v24 + 64)` in the decompiler output is `gametype_icon2->parameters.text_box_
 * parameters.string_list_index` (offset 0x40 = 64 decimal) — the decompiler lost the widget_instance type on
 * v24/v30 (declared as bare `int`) after `(int)v23->next`, so it fell back to raw pointer arithmetic; this is
 * the same field named explicitly on every sibling widget pointer (v17/v19/v20/v21) in the same function. v30
 * is always equal to v24 in the branch that assigns it (disasm confirms no divergent path), so it is dropped
 * as a separate local. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_client.h"
#include "headers/advertised_game_data.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_game_text_string.h"
#include "headers/blam_data_globals.h"


extern network_game_client *global_network_game_client_get(void);
extern uint32_t system_milliseconds(void);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern char *strstr(const char *haystack, const char *needle);

void server_list_menu_update(widget_instance *widget)
{
    advertised_game_data **server_slot;
    unsigned __int16 *text;
    __int16 string_list_index;
    widget_instance *children;

    /* re-zero the whole displayed_servers[9] array every update, regardless of the network state below */
    for ( int i = 0; i < 9; ++i )
        displayed_servers[i] = 0;

    if ( !global_network_game_client_get() )
        return;

    string_list_index = widget->parameters.text_box_parameters.string_list_index;
    widget->parameters.list_parameters.list_items = displayed_servers;
    widget->parameters.list_parameters.number_of_items = 0;
    if ( string_list_index > -1 )
        string_list_index = -1;
    children = widget->children;
    widget->parameters.text_box_parameters.string_list_index = string_list_index;

    if ( children )
    {
        server_slot = displayed_servers;
        do
        {
            if ( (int)server_slot >= (int)displayed_servers )   /* verified unreachable: see file header */
                break;

            text = ui_widget_realloc(children->parameters.text_box_parameters.text, 0x20u);
            children->parameters.text_box_parameters.text = text;
            if ( text )
            {
                if ( (*server_slot)->open == 1 )
                {
                    ustrncpy(text, (*server_slot)->name, 0xFu);
                }
                else
                {
                    int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
                    const wchar_t *status_prefix = unicode_string_list_get_string(text_tag, _string_closed_game_prefix);
                    usnprintf(children->parameters.text_box_parameters.text, 0xFu, L"%s %s", status_prefix,
                            (*server_slot)->name);
                }
                children->parameters.text_box_parameters.text[15] = 0;
            }
            children = children->next;
            ++server_slot;
        } while ( children );
    }

    unsigned int current_time = system_milliseconds();
    __int16 selected_index = widget->parameters.text_box_parameters.string_list_index;
    unsigned int elapsed_since_creation = current_time - widget->creation_time;

    widget_instance *game_type_icon = widget->parameters.list_parameters.extended_description->children;
    widget_instance *map_icon = game_type_icon->next;
    widget_instance *details_row = map_icon->next;
    widget_instance *status_text = details_row->children;
    widget_instance *info_widget = details_row->next;
    widget_instance *map_name_text = status_text->next;
    widget_instance *gametype_text = map_name_text->next;
    widget_instance *teams_text = gametype_text->next;
    widget_instance *player_count_text = teams_text->next;
    widget_instance *score_text = player_count_text->next;
    widget_instance *gametype_icon2 = score_text->next;

    if ( selected_index >= 0 )
    {
        advertised_game_data *selected_server = displayed_servers[selected_index];

        if ( selected_server->game_engine < first_usable_game_engine_index
          || selected_server->game_engine > last_usable_game_engine_index )
        {
            game_type_icon->animation_data.current_frame_index = 5;
        }
        else
        {
            switch ( selected_server->game_engine )
            {
                case game_engine_slayer:  game_type_icon->animation_data.current_frame_index = 2; break;
                case game_engine_oddball: game_type_icon->animation_data.current_frame_index = 3; break;
                case game_engine_king:    game_type_icon->animation_data.current_frame_index = 1; break;
                case game_engine_ctf:     game_type_icon->animation_data.current_frame_index = 0; break;
                default: /* game_engine_race */ game_type_icon->animation_data.current_frame_index = 4; break;
            }
        }

        __int16 map_icon_frame;
        if ( strstr(selected_server->map.name, "beavercreek") )
        {
            map_icon_frame = 0;
        }
        else if ( strstr(selected_server->map.name, "sidewinder") )
        {
            map_icon->animation_data.current_frame_index = 1;
            goto map_icon_set;
        }
        else if ( strstr(selected_server->map.name, "damnation") )
        {
            map_icon_frame = 2;
        }
        else if ( strstr(selected_server->map.name, "ratrace") )
        {
            map_icon->animation_data.current_frame_index = 3;
            goto map_icon_set;
        }
        else if ( strstr(selected_server->map.name, "prisoner") )
        {
            map_icon->animation_data.current_frame_index = 4;
            goto map_icon_set;
        }
        else if ( strstr(selected_server->map.name, "hangemhigh") )
        {
            map_icon->animation_data.current_frame_index = 5;
            goto map_icon_set;
        }
        else if ( strstr(selected_server->map.name, "chillout") )
        {
            map_icon->animation_data.current_frame_index = 6;
            goto map_icon_set;
        }
        else if ( strstr(selected_server->map.name, "carousel") )
        {
            map_icon->animation_data.current_frame_index = 7;
            goto map_icon_set;
        }
        else if ( strstr(selected_server->map.name, "boardingaction") )
        {
            map_icon->animation_data.current_frame_index = 8;
            goto map_icon_set;
        }
        else if ( strstr(selected_server->map.name, "bloodgulch") )
        {
            map_icon_frame = 9;
        }
        else if ( strstr(selected_server->map.name, "wizard") )
        {
            map_icon_frame = 10;
        }
        else if ( strstr(selected_server->map.name, "putput") )
        {
            map_icon_frame = 11;
        }
        else if ( strstr(selected_server->map.name, "longest") )
        {
            map_icon->animation_data.current_frame_index = 12;
            goto map_icon_set;
        }
        else
        {
            map_icon_frame = 19;
        }
        map_icon->animation_data.current_frame_index = map_icon_frame;

map_icon_set:
        __int16 status_string_index = 20;
        if ( selected_server->open != 1 )
            status_string_index = 21;
        status_text->parameters.text_box_parameters.string_list_index = status_string_index;
        map_name_text->parameters.text_box_parameters.string_list_index = map_icon->animation_data.current_frame_index;

        if ( selected_server->game_engine < first_usable_game_engine_index
          || selected_server->game_engine > last_usable_game_engine_index )
        {
            gametype_text->parameters.text_box_parameters.string_list_index = _string_unknown_game_type;
        }
        else
        {
            switch ( selected_server->game_engine )
            {
                case game_engine_slayer:  gametype_text->parameters.text_box_parameters.string_list_index = _string_slayer; break;
                case game_engine_oddball: gametype_text->parameters.text_box_parameters.string_list_index = _string_oddball; break;
                case game_engine_king:    gametype_text->parameters.text_box_parameters.string_list_index = _string_king_of_the_hill; break;
                case game_engine_ctf:     gametype_text->parameters.text_box_parameters.string_list_index = _string_capture_the_flag; break;
                default: /* game_engine_race */ gametype_text->parameters.text_box_parameters.string_list_index = _string_race; break;
            }
        }

        teams_text->parameters.text_box_parameters.string_list_index = (selected_server->teams_enabled == 1) ? _string_team_game : _string_free_for_all;

        unsigned __int16 *player_count_buffer =
                ui_widget_realloc(player_count_text->parameters.text_box_parameters.text, 8u);
        player_count_text->parameters.text_box_parameters.text = player_count_buffer;
        if ( player_count_buffer )
        {
            usnprintf(player_count_buffer, 3u, L"%d", selected_server->current_number_of_players);
            player_count_text->parameters.text_box_parameters.text[3] = 0;
        }

        unsigned __int16 *score_buffer =
                ui_widget_realloc(score_text->parameters.text_box_parameters.text, 8u);
        score_text->parameters.text_box_parameters.text = score_buffer;
        if ( score_buffer )
        {
            usnprintf(score_buffer, 3u, L"%d", selected_server->score_to_win);
            score_text->parameters.text_box_parameters.text[3] = 0;
        }

        if ( selected_server->game_engine < first_usable_game_engine_index
          || selected_server->game_engine > last_usable_game_engine_index )
        {
            gametype_icon2->parameters.text_box_parameters.string_list_index = _string_empty_string;
        }
        else if ( selected_server->game_engine == game_engine_slayer )
        {
            gametype_icon2->parameters.text_box_parameters.string_list_index = _string_frags;
        }
        else if ( selected_server->game_engine == game_engine_oddball )
        {
            /* terminator==1 goes straight to 24; terminator!=1 falls through (in the disasm) to the
             * same shared "*(v30+64)=23" store the game_engine==4 case below also falls through to */
            gametype_icon2->parameters.text_box_parameters.string_list_index =
                    (selected_server->terminator == 1) ? _string_frags : _string_minutes;
        }
        else if ( selected_server->game_engine == game_engine_king )
        {
            gametype_icon2->parameters.text_box_parameters.string_list_index = _string_minutes;
        }
        else if ( selected_server->game_engine == game_engine_ctf )
        {
            gametype_icon2->parameters.text_box_parameters.string_list_index = _string_captures;
        }
        else
        {
            gametype_icon2->parameters.text_box_parameters.string_list_index = _string_laps;
        }

        info_widget->parameters.text_box_parameters.string_list_index = 2;
        info_widget->visible = 0;

        if ( !widget->focused_child )
        {
            widget_instance *first_child = widget->children;
            widget->parameters.text_box_parameters.string_list_index = 0;
            widget->focused_child = first_child;
        }
        return;
    }

    /* no server selected (the only path this compiled binary ever actually takes) */
    game_type_icon->animation_data.current_frame_index = 5;
    map_icon->animation_data.current_frame_index = 19;
    status_text->parameters.text_box_parameters.string_list_index = 1;
    map_name_text->parameters.text_box_parameters.string_list_index = 20;
    gametype_text->parameters.text_box_parameters.string_list_index = 1;
    teams_text->parameters.text_box_parameters.string_list_index = 1;

    unsigned __int16 *player_count_buffer =
            ui_widget_realloc(player_count_text->parameters.text_box_parameters.text, 8u);
    player_count_text->parameters.text_box_parameters.text = player_count_buffer;
    if ( player_count_buffer )
        *player_count_buffer = 0;

    unsigned __int16 *score_buffer =
            ui_widget_realloc(score_text->parameters.text_box_parameters.text, 8u);
    score_text->parameters.text_box_parameters.text = score_buffer;
    if ( score_buffer )
        *score_buffer = 0;

    gametype_icon2->parameters.text_box_parameters.string_list_index = _string_empty_string;
    info_widget->parameters.text_box_parameters.string_list_index = elapsed_since_creation >= 0x3E8;
    info_widget->visible = 1;
}
