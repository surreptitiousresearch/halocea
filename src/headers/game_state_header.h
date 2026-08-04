#pragma once

/* On-disk / in-memory game state header block (0x14C = 332 bytes). */
typedef struct game_state_header
{
    unsigned int allocation_size_checksum; // 0x000
    char map_name[256];                    // 0x004
    char build_number[32];                 // 0x104
    short player_count;                    // 0x124
    short difficulty;                      // 0x126
    unsigned int cache_file_checksum;      // 0x128
    int unused[7];                         // 0x12C
    unsigned int checksum;                 // 0x148
} game_state_header;                        // sizeof == 0x14C
