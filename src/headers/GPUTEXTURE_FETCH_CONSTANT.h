#pragma once

#include <stdint.h>
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
    uint32_t Type : 2;
    uint32_t SignX : 2;
    uint32_t SignY : 2;
    uint32_t SignZ : 2;
    uint32_t SignW : 2;
    uint32_t ClampX : 3;
    uint32_t ClampY : 3;
    uint32_t ClampZ : 3;
    uint32_t : 3;
    uint32_t Pitch : 9;
    uint32_t Tiled : 1;
    /* dword 1 */
    uint32_t DataFormat : 6;
    uint32_t Endian : 2;
    uint32_t RequestSize : 2;
    uint32_t Stacked : 1;
    uint32_t ClampPolicy : 1;
    uint32_t BaseAddress : 20;
    /* dword 2 — DB member type GPUTEXTURE_FETCH_CONSTANT::<unnamed_tag>::<unnamed_type_Size>
     * (unnamed union; the $-tag below is a local spelling for the DB's unnamed tag) */
    union $GPUTEXTURE_FETCH_CONSTANT_Size {
        GPUTEXTURESIZE_1D    OneD;
        GPUTEXTURESIZE_2D    TwoD;
        GPUTEXTURESIZE_3D    ThreeD;
        GPUTEXTURESIZE_STACK Stack;
    } Size;
    /* dword 3 */
    uint32_t NumFormat : 1;
    uint32_t SwizzleX : 3;
    uint32_t SwizzleY : 3;
    uint32_t SwizzleZ : 3;
    uint32_t SwizzleW : 3;
    int32_t          ExpAdjust : 6;
    uint32_t MagFilter : 2;
    uint32_t MinFilter : 2;
    uint32_t MipFilter : 2;
    uint32_t AnisoFilter : 3;
    uint32_t : 3;
    uint32_t BorderSize : 1;
    /* dword 4 */
    uint32_t VolMagFilter : 1;
    uint32_t VolMinFilter : 1;
    uint32_t MinMipLevel : 4;
    uint32_t MaxMipLevel : 4;
    uint32_t MagAnisoWalk : 1;
    uint32_t MinAnisoWalk : 1;
    int32_t          LODBias : 10;
    int32_t          GradExpAdjustH : 5;
    int32_t          GradExpAdjustV : 5;
    /* dword 5 */
    uint32_t BorderColor : 2;
    uint32_t ForceBCWToMax : 1;
    uint32_t TriClamp : 2;
    int32_t          AnisoBias : 4;
    uint32_t Dimension : 2;
    uint32_t PackedMips : 1;
    uint32_t MipAddress : 20;
} _83F7CCE36CB6F839A961843D35C7AA0B;

typedef union GPUTEXTURE_FETCH_CONSTANT
{
    struct _83F7CCE36CB6F839A961843D35C7AA0B __s0; /* 0x00 — packed bitfield view */
    unsigned int dword[6];                         /* 0x00 — raw dword view */
} GPUTEXTURE_FETCH_CONSTANT; /* 24 bytes */
