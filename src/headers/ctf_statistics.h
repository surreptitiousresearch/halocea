#pragma once
/* ctf_statistics — CTF arm of the multiplayer_statistics union (6 bytes, DB types_members-confirmed). */

typedef struct ctf_statistics
{
    __int16 flag_grabs;   /* 0x00 */
    __int16 flag_returns; /* 0x02 */
    __int16 flag_scores;  /* 0x04 */
} ctf_statistics; /* 6 bytes */
