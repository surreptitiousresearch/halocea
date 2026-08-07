/* input_abstraction_update @ 0x83726258 — sample auxiliary keyboard buttons, then route
 * input to the active mode handler (events / in-game xbox), or clear control settings.
 * active_modes is a bitset: 1=events only, 2=game, 4=clear-settings, 8=mode 3. */
#include <stdint.h>
#include <string.h>
#include "headers/input_abstraction_globals.h"
#include "headers/blam_data_globals.h"

extern uint32_t main_get_mseconds(void);
extern void input_key_debounce_update(void);
extern uint8_t input_key_is_down(int16_t key_code);
extern void input_mode_handle_events(void);
extern void input_mode_handle_game_xbox(void);

void input_abstraction_update(void) /* attested void: tail restgprlr, 0/1 callers consume r3 */
{
    unsigned int mseconds = main_get_mseconds();
    short *m;
    int i;
    int result = 0;
    int16_t mode_code;

    input_abstraction_globals.all_idle = 1;
    input_key_debounce_update();

    i = 0;
    for ( m = keyboard_auxbutton_mapping; m < &keyboard_auxbutton_mapping[3]; ++m )   /* DB-typed __int16[3] map */
    {
        result = input_key_is_down(*m);
        input_abstraction_globals.auxbutton_ticks[i++] = (unsigned char)result;
    }

    if ( input_abstraction_globals.active_modes == 1 )
        mode_code = 0;
    else if ( input_abstraction_globals.active_modes & 8 )
        mode_code = 3;
    else if ( input_abstraction_globals.active_modes & 4 )
        mode_code = 2;
    else
        mode_code = (input_abstraction_globals.active_modes & 2) == 0 ? 255 : 1;

    if ( (unsigned int)mode_code <= 3 )
    {
        if ( mode_code == 1 )
        {
            ignore_game_input_time = mseconds + 200;
            input_mode_handle_events();
            return;
        }
        else if ( mode_code == 2 )
        {
            /* recovered: byte-cursor walk from &player_control_settings[3].invert_look+4 (9 dwords,
             * 0x90..0xB4) -> input_state[0]; prior transcription (index [3*36], post-increment) was wrong */
            memset(&input_abstraction_globals.input_state[0], 0, sizeof(input_abstraction_globals.input_state[0]));
        }
        else if ( mode_code == 0 && mseconds > ignore_game_input_time )
        {
            input_mode_handle_game_xbox();
            return;
        }
    }
}
