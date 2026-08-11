/* virtual_keyboard_select @0x83783EF0 — commit the currently-highlighted virtual-keyboard key: ordinary
 * characters (and the '+' cell, which shares the same fallthrough) get inserted at the caret, with a
 * ".fortune" easter egg checked after every insert; '%'/'&'/''' toggle shift/caps/symbols; ')'/'\*' move the
 * caret; '$' commits/cancels the save-name dialog; '(' backspaces. Kept the original's goto-based shared
 * tails (insert-if-room-else-beep, then play-sound-and-update-shift) rather than duplicating the logic.
 *
 * DEVIATION: `caps_active`/`symbols_active` used a `_cntlzw(x) == 32` (clz-based zero test) idiom instead of
 * the `shift_active == 0` comparison the very next case uses for the same toggle — algebraically identical,
 * collapsed to the same `== 0` form. (A stale note here claimed event_manager_flush takes a `controller_index`
 * left stale by the caller; it does not — 0x837263F8-0x8372640C is void(void) and overwrites r3 with its own
 * memset Dst before using it, so the call takes no argument and the code below correctly passes none.)
 *
 * DEVIATION (#119): the return is `uint8_t`, not `int`. The sole caller narrows it to a byte before
 * testing — `mr r30, r3` @0x8378475C then `clrlwi r11, r30, 24` @0x83784760, `cmplwi cr6, r11, 1`
 * @0x83784764 — which is what MSVC emits only when the callee's declared return is byte-wide; the same
 * epilogue serves `bl virtual_keyboard_cancel` @0x83784758, already `uint8_t` here. Value-identical
 * (every path reaches the single `li r3, 1` @0x83784270). */

#include <stdint.h>
#include <string.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/error_type.h"
#include "headers/blam_data_globals.h"

extern const char virtual_keyboard_layout_table[5][11];

/* memmove declared by <string.h> */
extern unsigned int ustrlen(const wchar_t *string);
extern int ustrcmp(const wchar_t *string1, const wchar_t *string2);
extern void ustrcpy(wchar_t *dest, const wchar_t *src);
extern wchar_t virtual_keyboard_get_current_character(void);
extern uint32_t system_milliseconds(void);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern void display_error(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern uint8_t saved_game_file_name_unique(uint16_t *name);
extern uint8_t virtual_keyboard_cancel(void);
extern void virtual_keyboard_backspace(void);
extern void event_manager_flush(void);

uint8_t virtual_keyboard_select(void)
{
    int index = 11 * virtual_keyboard_globals.row + virtual_keyboard_globals.column;
    unsigned char key = virtual_keyboard_layout_table[0][index];
    int audio_feedback_sound;

    /* DEVIATION: unsigned range test (key - 36 > 7) unfolded — '$'..'+' are the layout table's
     * special-key sentinel characters the switch below handles */
    if ( key < '$' || key > '+' )
        goto insert_character;

    switch ( key )
    {
        case '%':
            ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
            virtual_keyboard_globals.shift_active = (virtual_keyboard_globals.shift_active == 0);
            goto update_shift;
        case '&':
            ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
            virtual_keyboard_globals.caps_active = (virtual_keyboard_globals.caps_active == 0);
            goto update_shift;
        case '\'':
            ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
            virtual_keyboard_globals.symbols_active = (virtual_keyboard_globals.symbols_active == 0);
            goto update_shift;
    }

    if ( key == '(' )
    {
        if ( virtual_keyboard_globals.first_key_replaces_buffer == 1 )
        {
            memset(virtual_keyboard_globals.text_buffer, 0, virtual_keyboard_globals.buffer_size);
            virtual_keyboard_globals.first_key_replaces_buffer = 0;
            virtual_keyboard_globals.insertion_point = virtual_keyboard_globals.text_buffer;
        }
        else
        {
            virtual_keyboard_backspace();
        }
        goto update_shift;
    }

    switch ( key )
    {
        case ')':
            if ( virtual_keyboard_globals.insertion_point > virtual_keyboard_globals.text_buffer )
                --virtual_keyboard_globals.insertion_point;
            audio_feedback_sound = _ui_audio_feedback_cursor;
            virtual_keyboard_globals.first_key_replaces_buffer = 0;
            goto play_sound;
        case '*':
            if ( *virtual_keyboard_globals.insertion_point )
                ++virtual_keyboard_globals.insertion_point;
            audio_feedback_sound = _ui_audio_feedback_cursor;
            virtual_keyboard_globals.first_key_replaces_buffer = 0;
            goto play_sound;
        case '$':
            if ( ustrcmp(virtual_keyboard_globals.original_buffer, virtual_keyboard_globals.text_buffer) )
            {
                if ( !*virtual_keyboard_globals.text_buffer )
                {
                    display_error(_error_cannot_create_saved_game_file_with_empty_name, -1, 1, 0);
                    virtual_keyboard_cancel();
                    goto finish_editing;
                }
                if ( !saved_game_file_name_unique(virtual_keyboard_globals.text_buffer) )
                {
                    display_error(_error_already_a_saved_game_file_with_that_name, -1, 1, 0);
                    virtual_keyboard_cancel();
                    goto finish_editing;
                }
            }
            virtual_keyboard_globals.buffer_saved = 1;
finish_editing:
            ui_play_audio_feedback_sound(_ui_audio_feedback_back);
            virtual_keyboard_globals.active = 0;
            event_manager_flush();
            goto update_shift;
    }

    /* default: replace-buffer-on-first-key, then insert a literal space if there's room */
    {
        uint16_t *text_buffer = virtual_keyboard_globals.text_buffer;
        if ( virtual_keyboard_globals.first_key_replaces_buffer == 1 )
        {
            memset(virtual_keyboard_globals.text_buffer, 0, virtual_keyboard_globals.buffer_size);
            text_buffer = virtual_keyboard_globals.text_buffer;
            virtual_keyboard_globals.first_key_replaces_buffer = 0;
            virtual_keyboard_globals.insertion_point = virtual_keyboard_globals.text_buffer;
        }

        int buffer_size = virtual_keyboard_globals.buffer_size;
        if ( (int)(buffer_size - 2 * (ustrlen(text_buffer) + 1)) < 2 )
            goto buffer_full;

        memmove(virtual_keyboard_globals.insertion_point + 1, virtual_keyboard_globals.insertion_point,
                (char *)virtual_keyboard_globals.text_buffer + buffer_size
                    - (char *)virtual_keyboard_globals.insertion_point - 2);
        *virtual_keyboard_globals.insertion_point++ = ' ';
        audio_feedback_sound = _ui_audio_feedback_forward;
        goto play_sound;
    }

insert_character:
    {
        uint16_t *text_buffer = virtual_keyboard_globals.text_buffer;
        if ( virtual_keyboard_globals.first_key_replaces_buffer == 1 )
        {
            memset(virtual_keyboard_globals.text_buffer, 0, virtual_keyboard_globals.buffer_size);
            text_buffer = virtual_keyboard_globals.text_buffer;
            virtual_keyboard_globals.first_key_replaces_buffer = 0;
            virtual_keyboard_globals.insertion_point = virtual_keyboard_globals.text_buffer;
        }

        int buffer_size = virtual_keyboard_globals.buffer_size;
        if ( (int)(buffer_size - 2 * (ustrlen(text_buffer) + 1)) < 2 )
            goto buffer_full;

        memmove(virtual_keyboard_globals.insertion_point + 1, virtual_keyboard_globals.insertion_point,
                (char *)virtual_keyboard_globals.text_buffer + buffer_size
                    - (char *)virtual_keyboard_globals.insertion_point - 2);
        *virtual_keyboard_globals.insertion_point++ = virtual_keyboard_get_current_character();

        if ( !ustrcmp(virtual_keyboard_globals.text_buffer, L".fortune") )
        {
            unsigned int fortune_index = system_milliseconds() % 10;
            if ( fortune_index > 9 )
                fortune_index = 9;
            virtual_keyboard_globals.caption_index = (int16_t)(fortune_index + 11);

            if ( virtual_keyboard_globals.original_buffer[0] )
            {
                ustrcpy(virtual_keyboard_globals.text_buffer, virtual_keyboard_globals.original_buffer);
                virtual_keyboard_globals.insertion_point =
                    &virtual_keyboard_globals.text_buffer[ustrlen(virtual_keyboard_globals.text_buffer)];
            }
            else
            {
                memset(virtual_keyboard_globals.text_buffer, 0, virtual_keyboard_globals.buffer_size);
                virtual_keyboard_globals.insertion_point = virtual_keyboard_globals.text_buffer;
            }
        }
        audio_feedback_sound = _ui_audio_feedback_forward;
        goto play_sound;
    }

buffer_full:
    audio_feedback_sound = _ui_audio_feedback_flag_failure;
play_sound:
    ui_play_audio_feedback_sound((int16_t)audio_feedback_sound);

update_shift:
    virtual_keyboard_globals.shift_active &=
        (virtual_keyboard_layout_table[virtual_keyboard_globals.row][virtual_keyboard_globals.column] != '%') - 1;
    return 1;
}
