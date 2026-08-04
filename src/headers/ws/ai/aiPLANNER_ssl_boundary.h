#pragma once
#include "aiPLANNER.h"
#include "aiPLANNER_DESC.h"
#include "../ia/iaIACTOR.h"
// Base/descriptor accessors for aiPLANNER's SSL-emitter methods. aiPLANNER's leading interface
// region (iface[200] @ offset 0) IS an iaIACTOR (aiPLANNER_IFACE : iaIACTOR adds only vtable, no
// data), so the inherited spDesc/sslObject the emitters dispatch through sit at offset 0. These
// inline accessors expose them WITHOUT altering the DB-verified aiPLANNER layout.
//
// spDesc.pointee is statically a dscDESC*; its dynamic type is the planner's aiPLANNER_DESC
// (single-inheritance chain aiPLANNER_DESC : iaDESC : sslDESC : dscDESC, all at offset 0). The
// per-species SSL function-id fields the emitters read (sslFuncId_*) live in aiPLANNER_DESC, whose
// DB-verified offsets match the decompiler's spDesc.pointee[N].<field> reads exactly.

static inline iaIACTOR &aiPLANNER_IA(aiPLANNER *self)
{
    return *reinterpret_cast<iaIACTOR *>(self);
}

static inline const aiPLANNER_DESC *aiPLANNER_Desc(aiPLANNER *self)
{
    return reinterpret_cast<const aiPLANNER_DESC *>(aiPLANNER_IA(self).spDesc.pointee);
}
