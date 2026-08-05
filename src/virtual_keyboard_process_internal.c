/* virtual_keyboard_process_internal @0x837842B0 — drains the pending input event queue for the on-screen
 * keyboard, resolving all events in this tick down to a single virtual_keyboard_event (`direction`:
 * _event_tab_left/right/up/down, _event_key_select, _event_cancel), then applies that action once: moves the row/column cursor
 * (skipping over layout cells that repeat the same character as the current cell), or invokes select/cancel.
 * Tab (button 8/9/0xA/0xB) events are debounced against `time_of_last_tab` (0xFA ms) unless the controller's
 * last recorded event differs or the button is a fresh press. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/event_record.h"
#include "headers/event_type.h"
#include "headers/gamepad_button.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/virtual_keyboard_event.h"
#include "headers/blam_data_globals.h"

extern uint32_t system_milliseconds(void);
extern uint8_t get_next_event(event_record *event, int16_t local_player_index);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern int virtual_keyboard_select(void);
extern uint8_t virtual_keyboard_cancel(void);
extern void virtual_keyboard_backspace(void);
extern const char virtual_keyboard_layout_table[5][11];
extern void *memset(void *dest, int value, unsigned int count);

void virtual_keyboard_process_internal(void)
{
    unsigned int now = system_milliseconds();
    int direction = -1;
    event_record event;

    while (get_next_event(&event, -1))
    {
        if (event.type == event_type_left_analog_stick)
        {
            if (event.value.analog_value.n[1] == 0x7FFF)
                direction = _event_tab_up;
            else if (event.value.analog_value.n[1] == (int16_t)0x8000)
                direction = _event_tab_down;
            else if (event.value.analog_value.n[0] == (int16_t)0x8000)
                direction = _event_tab_left;
            else if (event.value.analog_value.n[0] == 0x7FFF)
                direction = _event_tab_right;
        }
        else if (event.type == event_type_button)
        {
            switch (event.value.button_value.button)
            {
            case _gamepad_analog_button_a:
                if (event.value.button_value.value == 1)
                    direction = _event_key_select;
                break;

            case _gamepad_analog_button_b:
            case _gamepad_binary_button_back:
                if (event.value.button_value.value == 1)
                    direction = _event_cancel;
                break;

            case _gamepad_analog_button_x:
                if (event.value.button_value.value == 1)
                {
                    if (virtual_keyboard_globals.first_key_replaces_buffer == 1)
                    {
                        memset(virtual_keyboard_globals.text_buffer, 0, virtual_keyboard_globals.buffer_size);
                        virtual_keyboard_globals.first_key_replaces_buffer = 0;
                        virtual_keyboard_globals.insertion_point = virtual_keyboard_globals.text_buffer;
                        ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
                    }
                    else
                    {
                        virtual_keyboard_backspace();
                    }
                }
                break;

            case _gamepad_analog_button_left_trigger: /* move insertion point left */
                if (event.value.button_value.value == 1)
                {
                    virtual_keyboard_globals.first_key_replaces_buffer = 0;
                    if (virtual_keyboard_globals.insertion_point > virtual_keyboard_globals.text_buffer)
                        --virtual_keyboard_globals.insertion_point;
                    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
                }
                break;

            case _gamepad_analog_button_right_trigger: /* move insertion point right */
                if (event.value.button_value.value == 1)
                {
                    virtual_keyboard_globals.first_key_replaces_buffer = 0;
                    if (*virtual_keyboard_globals.insertion_point)
                        ++virtual_keyboard_globals.insertion_point;
                    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
                }
                break;

            case _gamepad_binary_button_dpad_up: /* tab up (debounced) */
                if (virtual_keyboard_globals.last_event != 2
                    || now - time_of_last_tab >= 0xFA
                    || event.value.button_value.value == 1)
                {
                    direction = _event_tab_up;
                    time_of_last_tab = now;
                }
                break;

            case _gamepad_binary_button_dpad_down: /* tab down (debounced) */
                if (virtual_keyboard_globals.last_event != 3
                    || now - time_of_last_tab >= 0xFA
                    || event.value.button_value.value == 1)
                {
                    direction = _event_tab_down;
                    time_of_last_tab = now;
                }
                break;

            case _gamepad_binary_button_dpad_left: /* tab left (debounced) */
                if (virtual_keyboard_globals.last_event != 0
                    || now - time_of_last_tab >= 0xFA
                    || event.value.button_value.value == 1)
                {
                    direction = _event_tab_left;
                    time_of_last_tab = now;
                }
                break;

            case _gamepad_binary_button_dpad_right: /* tab right (debounced) */
                if (virtual_keyboard_globals.last_event != 1
                    || now - time_of_last_tab >= 0xFA
                    || event.value.button_value.value == 1)
                {
                    time_of_last_tab = now;
                    direction = _event_tab_right;
                }
                break;

            case _gamepad_binary_button_start: /* explicit select-key shortcut: jump cursor to top-left, then select */
                if (event.value.button_value.value == 1)
                {
                    virtual_keyboard_globals.row = 0;
                    direction = _event_key_select;
                    virtual_keyboard_globals.column = 0;
                }
                break;

            default:
                break;
            }
        }
    }

    if (direction == -1)
        return;

    int16_t row               = virtual_keyboard_globals.row;
    int16_t column            = virtual_keyboard_globals.column;
    int     row_stride        = NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS * row;
    char    current_key       = virtual_keyboard_layout_table[0][column + row_stride];
    virtual_keyboard_globals.last_key = current_key;

    uint8_t acted = 0;

    switch (direction)
    {
    case _event_tab_right: /* advance column, skipping cells identical to current key */
        do
        {
            column = ((int16_t)(column + 1) == NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS) ? 0 : column + 1;
            virtual_keyboard_globals.column = column;
        }
        while (virtual_keyboard_layout_table[0][column + row_stride] == current_key);
        break;

    case _event_tab_up: /* retreat row, skipping cells identical to current key */
        do
        {
            row = (int16_t)(row - 1);
            virtual_keyboard_globals.row = row;
            if (row < 0)
            {
                row = NUMBER_OF_VIRTUAL_KEYBOARD_ROWS - 1;
                virtual_keyboard_globals.row = NUMBER_OF_VIRTUAL_KEYBOARD_ROWS - 1;
            }
        }
        while (virtual_keyboard_layout_table[row][column] == current_key);
        break;

    case _event_tab_down: /* advance row, skipping cells identical to current key */
        do
        {
            row = ((int16_t)(row + 1) == NUMBER_OF_VIRTUAL_KEYBOARD_ROWS) ? 0 : row + 1;
            virtual_keyboard_globals.row = row;
        }
        while (virtual_keyboard_layout_table[row][column] == current_key);
        break;

    case _event_key_select:
        acted = virtual_keyboard_select();
        if (acted == 1)
        {
            virtual_keyboard_globals.time_of_last_event = now;
            virtual_keyboard_globals.last_event = direction;
        }
        return;

    default: /* _event_cancel; _event_tab_left falls through to "left" below */
        if (direction != _event_tab_left)
        {
            acted = virtual_keyboard_cancel();
            if (acted == 1)
            {
                virtual_keyboard_globals.time_of_last_event = now;
                virtual_keyboard_globals.last_event = direction;
            }
            return;
        }

        /* left: retreat column, skipping cells identical to current key */
        do
        {
            column = (int16_t)(column - 1);
            virtual_keyboard_globals.column = column;
            if (column < 0)
            {
                column = NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS - 1;
                virtual_keyboard_globals.column = NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS - 1;
            }
        }
        while (virtual_keyboard_layout_table[0][column + row_stride] == current_key);
        break;
    }

    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
    virtual_keyboard_globals.time_of_last_event = now;
    virtual_keyboard_globals.last_event = direction;
}
