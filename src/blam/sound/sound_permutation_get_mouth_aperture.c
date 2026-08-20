/* sound_permutation_get_mouth_aperture @0x837F8AB0 */
#include <stdint.h>
#include "headers/sound_permutation.h"
#include "headers/tag_data.h"

uint8_t * sound_permutation_get_mouth_aperture(const sound_permutation *permutation, int16_t tick_index)
{
    return (unsigned char *)permutation->mouth_data.address + tick_index;
}
