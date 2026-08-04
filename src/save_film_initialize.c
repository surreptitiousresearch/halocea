/* save_film_initialize @ 0x836EB1E0 — reset film record/playback state */

#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/blam_data_globals.h"

#include "headers/data_packet_group_definition.h"
extern void data_packet_group_initialize(data_packet_group_definition *group);
extern void saved_film_flush_buffer(void);

void save_film_initialize(void) /* attested void: tail-call of void fn, 0/1 callers consume r3 */
{
    data_packet_group_initialize(&saved_film_group);
    global_saved_film.file = 0;
    global_saved_film.mode = _saved_film_inactive_mode;
    saved_film_flush_buffer();
}
