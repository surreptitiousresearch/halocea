/* splitscreen_pregame_status_screen_update @0x8377EC80 — refresh the pre-game "status" UI widget for a
 * split-screen network game: a countdown timer line, a game-state line, and one name/team-color entry per local
 * player. It reallocates each text-box's string buffer to fit, fills the timer with the "-:--" placeholder,
 * blanks the status line, finds the (up to two) local-machine players and records their slot for the controller
 * they own, then for each on-screen entry writes that player's name and, in team games, picks the controller-
 * color bitmap frame and label string from the player's team (blanking the entry when no local player owns it).
 *
 * DEVIATION: two Hex-Rays renderings are decoded here. (1) `HIBYTE(game->name[16*(slot+11)])` is an array-overrun
 * pun — that wchar lands on game->players[slot].team_index (@0x1E), so it is read directly. (2) The v28/v29 stack
 * pair is aliased: v29[-2]/v29[-1] (inside the v28 __int64) hold the player slot chosen for local controllers
 * 0/1, while v29[0..2] hold three widget pointers; both are folded into one `slots[]` array (slots[k] == v29[k-2]).
 * The controller-bitmap walk is bounded by the address of the "longest" string (the table is immediately followed
 * by it in the data segment) — reproduced literally; the loop yields two iterations in practice. The "?" scratch
 * string (a mutable UI buffer Hex-Rays labels as the literal L"?") is filled from the unnamed global
 * empty_wide_string and then copied into the status line. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_data.h"
#include "headers/network_player.h"

extern network_game_data *network_game_get_game(void);
typedef struct network_game_client network_game_client;
extern network_game_client *global_network_game_client_get(void);   /* result unused here */
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrcpy(wchar_t *dest, const wchar_t *src);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern unsigned int ustrlen(const wchar_t *string);
extern uint8_t network_player_is_valid(network_player *player);
extern void widget_instance_set_visibility_recursive(widget_instance *widget, uint8_t visible);

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */
extern const uint8_t indexed_controller_bitmaps[][3];

void splitscreen_pregame_status_screen_update(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();
    if ( !game )
        return;

    global_network_game_client_get();
    widget_instance *children = widget->children;
    widget_instance *status_line = children->next;
    widget_instance *timer_widget = status_line->next;

    uint16_t *timer_text = ui_widget_realloc(
            timer_widget->parameters.text_box_parameters.text, 0x20);
    timer_widget->parameters.text_box_parameters.text = timer_text;
    if ( timer_text )
    {
        global_network_game_client_get();
        ustrncpy(timer_widget->parameters.text_box_parameters.text, L"-:--", 0xF);
        status_line->visible = 1;
        status_line->parameters.text_box_parameters.string_list_index = 1;
        uint16_t *text = timer_widget->parameters.text_box_parameters.text;
        timer_widget->visible = 0;
        text[15] = 0;
    }

    widget_instance *status_root = children->children;
    widget_instance *status_root_next = status_root->next;

    /* slots[k] == v29[k-2]: slots[0]/[1] are the player slots owned by local controllers
       0/1 (the aliased v28 __int64, initialized to -1); slots[2..4] are widget pointers. */
    int slots[8];
    slots[0] = -1;
    slots[1] = -1;
    slots[2] = (int)status_root_next->next;
    slots[3] = (int)((widget_instance *)slots[2])->next;
    slots[4] = (int)((widget_instance *)slots[2])->next->next;

    ustrcpy(L"?", empty_wide_string);

    int local_count = 0;
    for ( int player_slot = 0; player_slot < 32; ++player_slot )
    {
        network_player *player = &game->players[player_slot];
        if ( network_player_is_valid(player) && player->machine_index == 0 )
        {
            ++local_count;
            slots[player->controller_index] = player_slot;
            if ( local_count == 2 )
                break;
        }
    }

    unsigned int status_length = ustrlen(L"?");
    uint16_t *status_text = ui_widget_realloc(
            status_root->parameters.text_box_parameters.text, 2 * status_length + 2);
    status_root->parameters.text_box_parameters.text = status_text;
    if ( status_text )
    {
        ustrncpy(status_text, L"?", status_length);
        status_root->parameters.text_box_parameters.text[status_length] = 0;
    }
    status_root_next->animation_data.current_frame_index = 0;

    const uint8_t *controller_bitmap = &indexed_controller_bitmaps[0][2];
    for ( int i = 0; ; ++i )
    {
        widget_instance *entry = (widget_instance *)slots[i + 2];
        widget_instance *name_widget = entry->children;
        widget_instance *icon_widget = name_widget->next;
        widget_instance *label_widget = icon_widget->next;

        if ( !game->variant.universal_variant.teams )
            widget_instance_set_visibility_recursive(icon_widget->next, 0);

        int player_slot = slots[i];
        if ( player_slot == -1 )
        {
            name_widget->animation_data.current_frame_index = 0;
            uint16_t *empty = ui_widget_realloc(
                    icon_widget->parameters.text_box_parameters.text, 2);
            icon_widget->parameters.text_box_parameters.text = empty;
            if ( empty )
                *empty = 0;
            label_widget->parameters.text_box_parameters.string_list_index = 0;
        }
        else
        {
            unsigned int name_length = ustrlen(game->players[player_slot].name);
            uint16_t *name_text = ui_widget_realloc(
                    icon_widget->parameters.text_box_parameters.text, 2 * name_length + 2);
            icon_widget->parameters.text_box_parameters.text = name_text;
            if ( name_text )
            {
                ustrncpy(name_text, game->players[player_slot].name, name_length);
                icon_widget->parameters.text_box_parameters.text[name_length] = 0;
            }

            if ( game->variant.universal_variant.teams )
            {
                char team = game->players[player_slot].team_index;
                if ( team == 0 )
                {
                    name_widget->animation_data.current_frame_index = controller_bitmap[-1];
                    label_widget->parameters.text_box_parameters.string_list_index = 0;
                }
                else if ( team != 1 )
                {
                    name_widget->animation_data.current_frame_index = controller_bitmap[-2];
                    label_widget->parameters.text_box_parameters.string_list_index = 0;
                }
                else
                {
                    name_widget->animation_data.current_frame_index = controller_bitmap[0];
                    label_widget->parameters.text_box_parameters.string_list_index = 1;
                }
            }
            else
            {
                name_widget->animation_data.current_frame_index = 1;
            }
        }

        controller_bitmap += 3;
        if ( (const char *)controller_bitmap >= (const char *)"longest" )
            return;
    }
}
