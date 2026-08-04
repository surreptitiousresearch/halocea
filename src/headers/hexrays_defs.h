#pragma once
/* hexrays_defs — minimal word-extraction helpers matching the Hex-Rays decompiler's LODWORD/HIWORD
 * idioms. The Blam shader code frequently type-puns through the generic `_shader` base array to read
 * integer/short fields of the concrete shader subtype that overlaps the base's float members; these
 * macros reproduce those reads faithfully. */

#define LODWORD(x)  (*(int *)&(x))
#define HIDWORD(x)  (*((int *)&(x) + 1))
#define LOWORD(x)   (*(unsigned __int16 *)&(x))
#define LOBYTE(x)   (*(unsigned __int8 *)&(x))
#define HIBYTE(x)   (*((unsigned __int8 *)&(x) + 1))
#define HIWORD(x)   (*((unsigned __int16 *)&(x) + 1))
#define SHIWORD(x)  (*((signed __int16 *)&(x) + 1))
#define COERCE_FLOAT(x) (*(float *)&(x))
