#pragma once
// DB-verified via types_members _CFCD6AE9FC011AC9A63B1394A3C3FE62 (anonymous PDB type)

#include <stdint.h>
#include "game_state_header.h"

/* Global descriptor for the game-state memory arena.
   IDA type: anonymous struct _CFCD6AE9FC011AC9A63B1394A3C3FE62 */
struct game_state_globals
{
    void *base_address;                // 0x00 base of the allocated arena
    int cpu_allocation_size;           // 0x04 bytes handed out to CPU-side allocations
    int gpu_allocation_size;           // 0x08 bytes handed out to GPU-side allocations
    uint32_t allocation_size_checksum; // 0x0C running CRC over allocation sizes (crc_* family operates on uint32_t)
    unsigned char locked;              // 0x10
    unsigned char saved_game_valid;    // 0x11
    unsigned char _pad12[2];           // 0x12
    int revert_time;                   // 0x14
    game_state_header *header;         // 0x18
};

extern struct game_state_globals game_state_globals;
