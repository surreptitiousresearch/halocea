/* sound_valid_for_channel @ 0x83714F68 — does a sound's format match what a hardware channel of a
 * given type can play? The channel's type flags encode the expected format in their bits:
 *   bit0 = spatialized, bit1 = clear when encoding is mono(0), bit2 = high sample rate,
 *   bit3 = clear when uncompressed. A sound is valid when its sample rate, encoding and compression
 * all agree with those bits; for non-"any-spatialization" channels (bit1 of the *type* clear) the
 * spatialization must also match bit0.
 *
 * Deviation: the decompiler rendered the final spatialization check as a __PAIR64__ borrow trick;
 * it is reproduced here as the equivalent direct comparison ((spatialization != 0) == (flags & 1)). */
#include <stdint.h>
#include "headers/spatialization_mode.h"
#include "headers/sound_channel_flags.h"

/* attested: all five params extsh on entry (int16_t); sole caller byte-normalizes the
 * return (clrlwi r3,24 @ 0x83715A20) => uint8_t */
uint8_t sound_valid_for_channel(int16_t compression, int16_t encoding, int16_t sample_rate,
                                int16_t spatialization_mode, int16_t channel_type_flags)
{
    int result = sample_rate == ((channel_type_flags >> _sound_channel_44k_bit) & 1)
              && (encoding == 0) == (((unsigned int)~channel_type_flags >> _sound_channel_stereo_bit) & 1)
              && (compression == 0) == (((unsigned int)~channel_type_flags >> _sound_channel_compressed_bit) & 1);

    if ( (channel_type_flags & (1u << _sound_channel_stereo_bit)) == 0 )
        return ((spatialization_mode != _sound_spatialization_mode_none) == (channel_type_flags & (1u << _sound_channel_3d_bit))) ? result : 0;
    return result;
}
