#pragma once

#include "GPUTEXTURESIZE_1D.h"
#include "GPUTEXTURESIZE_2D.h"
#include "GPUTEXTURESIZE_3D.h"
#include "GPUTEXTURESIZE_STACK.h"

/* GPUTEXTURE_FETCH_CONSTANT — the Xenos GPU texture-fetch descriptor (external GPU/D3D boundary
 * type). DB: union, size 24: an anonymous packed-bitfield view (_83F7CCE36CB6F839A961843D35C7AA0B
 * __s0) overlaid with the raw dword[6] view. The $-name is the DB's anonymous-struct name, kept
 * verbatim ($ in identifiers is an MSVC extension). */

/* DB struct _83F7CCE36CB6F839A961843D35C7AA0B — bitfield view, 24 bytes (192 bits). */
typedef struct _83F7CCE36CB6F839A961843D35C7AA0B
{
    /* dword 0 */
    unsigned __int32 Type : 2;
    unsigned __int32 SignX : 2;
    unsigned __int32 SignY : 2;
    unsigned __int32 SignZ : 2;
    unsigned __int32 SignW : 2;
    unsigned __int32 ClampX : 3;
    unsigned __int32 ClampY : 3;
    unsigned __int32 ClampZ : 3;
    unsigned __int32 : 3;
    unsigned __int32 Pitch : 9;
    unsigned __int32 Tiled : 1;
    /* dword 1 */
    unsigned __int32 DataFormat : 6;
    unsigned __int32 Endian : 2;
    unsigned __int32 RequestSize : 2;
    unsigned __int32 Stacked : 1;
    unsigned __int32 ClampPolicy : 1;
    unsigned __int32 BaseAddress : 20;
    /* dword 2 — DB member type GPUTEXTURE_FETCH_CONSTANT::<unnamed_tag>::<unnamed_type_Size>
     * (unnamed union; the $-tag below is a local spelling for the DB's unnamed tag) */
    union $GPUTEXTURE_FETCH_CONSTANT_Size {
        GPUTEXTURESIZE_1D    OneD;
        GPUTEXTURESIZE_2D    TwoD;
        GPUTEXTURESIZE_3D    ThreeD;
        GPUTEXTURESIZE_STACK Stack;
    } Size;
    /* dword 3 */
    unsigned __int32 NumFormat : 1;
    unsigned __int32 SwizzleX : 3;
    unsigned __int32 SwizzleY : 3;
    unsigned __int32 SwizzleZ : 3;
    unsigned __int32 SwizzleW : 3;
    __int32          ExpAdjust : 6;
    unsigned __int32 MagFilter : 2;
    unsigned __int32 MinFilter : 2;
    unsigned __int32 MipFilter : 2;
    unsigned __int32 AnisoFilter : 3;
    unsigned __int32 : 3;
    unsigned __int32 BorderSize : 1;
    /* dword 4 */
    unsigned __int32 VolMagFilter : 1;
    unsigned __int32 VolMinFilter : 1;
    unsigned __int32 MinMipLevel : 4;
    unsigned __int32 MaxMipLevel : 4;
    unsigned __int32 MagAnisoWalk : 1;
    unsigned __int32 MinAnisoWalk : 1;
    __int32          LODBias : 10;
    __int32          GradExpAdjustH : 5;
    __int32          GradExpAdjustV : 5;
    /* dword 5 */
    unsigned __int32 BorderColor : 2;
    unsigned __int32 ForceBCWToMax : 1;
    unsigned __int32 TriClamp : 2;
    __int32          AnisoBias : 4;
    unsigned __int32 Dimension : 2;
    unsigned __int32 PackedMips : 1;
    unsigned __int32 MipAddress : 20;
} _83F7CCE36CB6F839A961843D35C7AA0B;

typedef union GPUTEXTURE_FETCH_CONSTANT
{
    struct _83F7CCE36CB6F839A961843D35C7AA0B __s0; /* 0x00 — packed bitfield view */
    unsigned int dword[6];                         /* 0x00 — raw dword view */
} GPUTEXTURE_FETCH_CONSTANT; /* 24 bytes */
