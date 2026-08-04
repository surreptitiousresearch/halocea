/* scripted_foley_predict @0x83713C30 — preload all sound permutations referenced by a looping_sound
 * definition's track list (looping_sound_track records, 160 bytes each, at tracks.address/+0x40, count at
 * tracks.count/+0x3C). For each referenced loop_sound tag that is a simple ('snd!' type 1) definition with at
 * least one pitch range, the permutation list is sent to the sound cache for prediction. */

#include "headers/global_tag_instances.h"
#include "headers/sound_permutation.h"
#include "headers/sound_definition.h"
#include "headers/sound_pitch_range.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/blam_data_globals.h"

/* real DB signature (0x837E7E8C): int __fastcall _sound_cache_sound_request(sound_permutation*, unsigned __int8 block, unsigned __int8 load, unsigned __int8 reference); */
extern int _sound_cache_sound_request(sound_permutation *sound, unsigned char block, unsigned char load, unsigned char reference);

void scripted_foley_predict(int definition_index)
{
    if ( definition_index == -1 )
        return;

    const looping_sound_definition *tag_data =
        TAG_GET(const looping_sound_definition, definition_index);
    int sound_count = tag_data->tracks.count;   /* +0x3C */
    for ( int i = 0; i < sound_count; i = (__int16)(i + 1) )
    {
        int sound_tag_index =
            ((const looping_sound_track *)tag_data->tracks.address)[i].loop_sound.index;  /* +0x40 base, 160-byte stride, tag index @+0x4C */
        if ( sound_tag_index == -1 )
            continue;

        sound_definition *sound_data = TAG_GET(sound_definition, sound_tag_index);
        if ( sound_data->pitch_ranges.count != 1 )
            continue;

        sound_pitch_range *pitch_range = (sound_pitch_range *)sound_data->pitch_ranges.address;
        if ( pitch_range->permutations.count )
            _sound_cache_sound_request((sound_permutation *)pitch_range->permutations.address, 0, 1u, 0);
    }
}
