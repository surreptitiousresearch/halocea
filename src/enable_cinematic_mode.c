/* enable_cinematic_mode @0x836B5B80 */
#include <stdint.h>
#include "headers/blam_data_globals.h"
/* DEVIATION: HALO_SOUND_SYSTEM is an opaque hcex C++ class (canonical forward-decl in
 * blam_data_globals.h). Model the isCinematicMode byte at offset 0x12 via a local layout view
 * and cast, rather than re-typedef'ing the opaque type. */
struct halo_sound_system_cinematic_view { unsigned char _pad[0x12]; unsigned char isCinematicMode; };

void enable_cinematic_mode(uint8_t enable)
{
    ((struct halo_sound_system_cinematic_view *)haloSoundSystem)->isCinematicMode = (enable != 0);
}
