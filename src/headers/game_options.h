#pragma once
/* game_options @ 268 (0x10C) bytes — parameters for loading a new map/game (main.c, game.c). */

typedef struct game_options
{
    unsigned int  flags;         /* 0x00 */
    __int16       code_version;  /* 0x04 */
    __int16       difficulty;    /* 0x06 */
    unsigned int  random_seed;   /* 0x08 */
    char          map_name[256]; /* 0x0C */
} game_options;
