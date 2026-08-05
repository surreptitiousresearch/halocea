#pragma once
#include <stdint.h>
// ws-engine anim: one level-of-detail definition record inside an animTPL (template).
// DB-verified layout (types_members tplLOD_DEF) — size 4.

typedef struct tplLOD_DEF {
    uint16_t objId;     // 0x00 index into the instance object list this LOD swaps in
    uint8_t  index;     // 0x02 LOD level this object belongs to
    uint8_t  isLastLod; // 0x03 when set, the object also fills every higher LOD level
} tplLOD_DEF;
