#pragma once
// DB-verified via types_members _3CF6219D685A7A2237831C031825DFD8 (anonymous PDB type)

/* Xbox-specific game-state backing-store descriptor (buffer + save-file paths).
   IDA type: anonymous struct _3CF6219D685A7A2237831C031825DFD8 */
/* NOTE: object name shadows the type name; keep it a bare struct TAG so the
   variable `xbox_game_state_globals` can share the identifier (consumers use the
   variable, never the type). */
struct xbox_game_state_globals
{
    unsigned char buffer_allocated;    // 0x000
    unsigned char _pad001[3];          // 0x001
    void *buffer;                      // 0x004
    int buffer_size;                   // 0x008
    unsigned char file_open;           // 0x00C
    unsigned char file_valid_for_read; // 0x00D
    unsigned char _pad00E[2];          // 0x00E
    void *handle;                      // 0x010
    char saved_game_path[256];         // 0x014
    char core_path[256];               // 0x114
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct xbox_game_state_globals xbox_game_state_globals;

#ifdef __cplusplus
}
#endif
