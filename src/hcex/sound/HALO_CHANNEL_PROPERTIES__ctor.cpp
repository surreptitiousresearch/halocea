#include "../headers/hcex/HALO_CHANNEL_PROPERTIES.h"

#include <string.h> // memset — CRT boundary

// HALO_CHANNEL_PROPERTIES::HALO_CHANNEL_PROPERTIES() @ 0x836B4F30
// Zero-fills the whole per-call playback-properties struct.
HALO_CHANNEL_PROPERTIES::HALO_CHANNEL_PROPERTIES()
{
    memset(this, 0, sizeof(HALO_CHANNEL_PROPERTIES));
}
