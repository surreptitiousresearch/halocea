#pragma once
// ws-engine anim: one level-of-detail definition record inside an animTPL (template).
// DB-verified layout (types_members tplLOD_DEF) — size 4.

typedef struct tplLOD_DEF {
    unsigned __int16 objId;     // 0x00 index into the instance object list this LOD swaps in
    unsigned __int8  index;     // 0x02 LOD level this object belongs to
    unsigned __int8  isLastLod; // 0x03 when set, the object also fills every higher LOD level
} tplLOD_DEF;
