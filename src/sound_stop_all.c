/* sound_stop_all @0x8371985C — stop every active sound: halt each playing sound, drop all looping
 * sounds, flush the platform mixer, and reset the scripted-dialog gate. No-op until sound init. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern void data_delete_all(data_array *data);
extern int data_next_index(const data_array *data, int16_t index);
extern void sound_stop(int sound_index);

void sound_stop_all(void)
{
    if ( sound_manager_globals.initialized )
    {
        for ( int index = data_next_index(sound_data, -1); index != -1; index = data_next_index(sound_data, index) )
            sound_stop(index);
        data_delete_all(looping_sound_data);
        sound_manager_globals.platform->flush();
    }
    sound_manager_globals.game_time_when_no_scripted_dialog_will_be_playing = 0;
}
