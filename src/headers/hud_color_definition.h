#pragma once
/* hud_color_definition — color + flash animation block for a HUD element (hud.c). Layout from the
 * database (32 bytes). The trailing `custom` slot is a 4-byte DB union (unused / objective arms;
 * $-names kept verbatim, an MSVC extension). */

/* DB struct _B497574040179F61B790D69CC44DAD4C (4 bytes) — objective arm of the custom union. */
typedef struct _B497574040179F61B790D69CC44DAD4C
{
    __int16 up_ticks;   /* 0x0 */
    __int16 fade_ticks; /* 0x2 */
} _B497574040179F61B790D69CC44DAD4C;

/* DB union _C7B85C91D7F2E2C299E1CAC96B1FEDFE (4 bytes) — hud_color_definition member `custom`. */
typedef union _C7B85C91D7F2E2C299E1CAC96B1FEDFE
{
    int unused;                                         /* 0x0 */
    struct _B497574040179F61B790D69CC44DAD4C objective; /* 0x0 */
} _C7B85C91D7F2E2C299E1CAC96B1FEDFE;

typedef struct hud_color_definition
{
    unsigned int  color;             /* 0x00 */
    unsigned int  flash_color;       /* 0x04 */
    float         flash_period;      /* 0x08 */
    float         flash_delay;       /* 0x0C */
    __int16       number_of_flashes; /* 0x10 */
    unsigned __int16 flash_flags;    /* 0x12 */
    float         flash_length;      /* 0x14 */
    unsigned int  disabled_color;    /* 0x18 */
    union _C7B85C91D7F2E2C299E1CAC96B1FEDFE custom; /* 0x1C */
} hud_color_definition;              /* 0x20 */
