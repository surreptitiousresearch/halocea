/* refresh_sound @ 0x83715830 — per-sound, per-frame validity check used while updating the channel
 * list. A sound is still valid (1) if it is delay-flagged, has no position-tracking proc, has not
 * yet reached its start time, or its tracking proc reports the source still exists. When the proc
 * fails for a one-shot impulse of a NON-speech class the sound is kept alive with its tracking proc
 * cleared; a speech-class sound (or a looping segment) whose proc failed is reported dead (0). */

#include <stddef.h>
#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_class_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_flags.h"
#include "headers/sound_type.h"
#include "headers/blam_data_globals.h"

/* attested: r3 first use clrlwi 16 @ 0x83715844 => uint16_t datum index; caller
 * refresh_sounds byte-normalizes the return (clrlwi r3,24 @ 0x83717418) => uint8_t */
uint8_t refresh_sound(int sound_index)
{
    sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    uint8_t (*track_proc)(int, const void *, sound_source *);

    if ( (datum->flags & (1u << _sound_delayed_bit)) != 0 )
        return 1;                                  /* delayed sound: start postponed, still valid */

    track_proc = datum->track_proc;
    if ( !track_proc
      || datum->start_time >= sound_manager_globals.render_time
      || track_proc(datum->source_identifier, datum->track_data, &datum->source) )
        return 1;

    if ( datum->type == _sound_impulse )           /* one-shot impulse, not a looping segment */
    {
        int class_index = TAG_GET(sound_definition, datum->definition_index)->class_index;
        if ( !sound_classes[class_index].speech )
        {
            /* non-speech: stop tracking but keep the sound alive. Disasm 0x837158F4 stores the
             * just-tested zero speech byte into the proc slot — i.e. clears it. */
            datum->track_proc = NULL;
            return 1;
        }
    }
    return 0;
}
