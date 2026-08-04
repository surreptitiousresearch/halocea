#pragma once
// Boundary accessors into aiPLANNER's owned species descriptor (the iaIACTOR base's spDesc.pointee)
// for the aiPLANNER "Nav*" SSL-dispatch batch (0x83251C50-0x832539xx). The descriptor's static type
// is dscDESC; its dynamic type is an aiPLANNER_DESC (the NEXT FRONTIER). Each public Nav* method
// reads one per-species SSL *function* id (a 4-byte word) out of the descriptor and dispatches it
// through sslObject.CallFunc — exactly the same idiom as aiBRAIN_DESC_sslfunc_boundary.h.
//
// The decompiler expresses each read as `spDesc.pointee[N].<field>` over a dscDESC[] view
// (sizeof(dscDESC) == 12; __vftable@0, dsREF_COUNT@4, pBrand@8); the absolute byte offsets below
// are N*12 + field-offset, matching each `lwz r?,off(r31)` site in the disassembly. Rather than
// fabricate the full aiPLANNER_DESC layout, these read the exact word each site reads.
//
// aiPLANNER models its iaIACTOR base as an opaque iface[200] blob, so spDesc (iaIACTOR+0x54) and
// sslObject (iaIACTOR+0x58) are reached by fixed byte offset rather than by named member.

#include "aiPLANNER.h"
#include "../dsc/dscDESC.h"
#include "../ssl/sslOBJ_REF.h"

// dsSMART_PTR<dscDESC>::pointee is the first (and only) word of spDesc @ iaIACTOR+0x54.
static inline dscDESC *aiPLANNER_SpDescPointee(const aiPLANNER *p) {
    return *reinterpret_cast<dscDESC *const *>(
        reinterpret_cast<const unsigned char *>(p) + 0x54);
}

// sslObject (the actor's script-object reference) @ iaIACTOR+0x58.
static inline sslOBJ_REF *aiPLANNER_SslObject(aiPLANNER *p) {
    return reinterpret_cast<sslOBJ_REF *>(
        reinterpret_cast<unsigned char *>(p) + 0x58);
}

static inline int aiPLANNER_DESC_SslFuncId(const dscDESC *desc, unsigned int byteOffset) {
    return *reinterpret_cast<const int *>(
        reinterpret_cast<const unsigned char *>(desc) + byteOffset);
}

// spDesc.pointee[14].__vftable == byte offset 168 — "NavLink"
static inline int aiPLANNER_DESC_SslFnNavLink(const dscDESC *d)             { return aiPLANNER_DESC_SslFuncId(d, 168); }
// spDesc.pointee[14].dsREF_COUNT == byte offset 172 — "NavLinkSingle"
static inline int aiPLANNER_DESC_SslFnNavLinkSingle(const dscDESC *d)       { return aiPLANNER_DESC_SslFuncId(d, 172); }
// spDesc.pointee[14].pBrand     == byte offset 176 — "NavUnLink"
static inline int aiPLANNER_DESC_SslFnNavUnLink(const dscDESC *d)           { return aiPLANNER_DESC_SslFuncId(d, 176); }
// spDesc.pointee[15].__vftable  == byte offset 180 — "NavUnLinkSingle"
static inline int aiPLANNER_DESC_SslFnNavUnLinkSingle(const dscDESC *d)     { return aiPLANNER_DESC_SslFuncId(d, 180); }
// spDesc.pointee[15].dsREF_COUNT == byte offset 184 — "NavBlockObj"
static inline int aiPLANNER_DESC_SslFnNavBlockObj(const dscDESC *d)         { return aiPLANNER_DESC_SslFuncId(d, 184); }
// spDesc.pointee[15].pBrand     == byte offset 188 — "NavUnBlockObj"
static inline int aiPLANNER_DESC_SslFnNavUnBlockObj(const dscDESC *d)       { return aiPLANNER_DESC_SslFuncId(d, 188); }
// spDesc.pointee[16].__vftable  == byte offset 192 — "NavDisableWP"
static inline int aiPLANNER_DESC_SslFnNavDisableWP(const dscDESC *d)        { return aiPLANNER_DESC_SslFuncId(d, 192); }
// spDesc.pointee[16].dsREF_COUNT == byte offset 196 — "NavEnableWP"
static inline int aiPLANNER_DESC_SslFnNavEnableWP(const dscDESC *d)         { return aiPLANNER_DESC_SslFuncId(d, 196); }
// spDesc.pointee[16].pBrand     == byte offset 200 — "NavForceAutolinkWP"
static inline int aiPLANNER_DESC_SslFnNavForceAutolinkWP(const dscDESC *d)  { return aiPLANNER_DESC_SslFuncId(d, 200); }
// spDesc.pointee[17].__vftable  == byte offset 204 — "NavRelinkWPToNS"
static inline int aiPLANNER_DESC_SslFnNavRelinkWPToNS(const dscDESC *d)     { return aiPLANNER_DESC_SslFuncId(d, 204); }
// spDesc.pointee[17].dsREF_COUNT == byte offset 208 — "NavSysMarkStatic"
static inline int aiPLANNER_DESC_SslFnNavSysMarkStatic(const dscDESC *d)    { return aiPLANNER_DESC_SslFuncId(d, 208); }
// spDesc.pointee[17].pBrand     == byte offset 212 — "NavSysPenalizeByDomain"
static inline int aiPLANNER_DESC_SslFnNavSysPenalizeByDomain(const dscDESC *d) { return aiPLANNER_DESC_SslFuncId(d, 212); }
// spDesc.pointee[18].__vftable  == byte offset 216 — "NavSysRemove"
static inline int aiPLANNER_DESC_SslFnNavSysRemove(const dscDESC *d)        { return aiPLANNER_DESC_SslFuncId(d, 216); }
// spDesc.pointee[18].dsREF_COUNT == byte offset 220 — "SetTeamsEnemies"
static inline int aiPLANNER_DESC_SslFnSetTeamsEnemies(const dscDESC *d)     { return aiPLANNER_DESC_SslFuncId(d, 220); }

// --- enemy/flag/group/death-smt SSL round-trip batch (0x832548B8-0x83255938) --------------------
// Same descriptor image (spDesc.pointee); offsets verified against `lwz r5,OFF(r31|r30)` at each site.
// spDesc.pointee[20].__vftable  == byte offset 240 — "FindGroupByName"
static inline int aiPLANNER_DESC_SslFnFindGroupByName(const dscDESC *d)     { return aiPLANNER_DESC_SslFuncId(d, 240); }
// spDesc.pointee[20].dsREF_COUNT == byte offset 244 — "CountLiveInDomain"
static inline int aiPLANNER_DESC_SslFnCountLiveInDomain(const dscDESC *d)   { return aiPLANNER_DESC_SslFuncId(d, 244); }
// spDesc.pointee[22].__vftable  == byte offset 264 — "UnregisterDeathSmtObject"
static inline int aiPLANNER_DESC_SslFnUnregisterDeathSmtObject(const dscDESC *d) { return aiPLANNER_DESC_SslFuncId(d, 264); }
// spDesc.pointee[22].dsREF_COUNT == byte offset 268 — "EnableStrike"
static inline int aiPLANNER_DESC_SslFnEnableStrike(const dscDESC *d)        { return aiPLANNER_DESC_SslFuncId(d, 268); }
// spDesc.pointee[22].pBrand     == byte offset 272 — "DisableStrike"
static inline int aiPLANNER_DESC_SslFnDisableStrike(const dscDESC *d)       { return aiPLANNER_DESC_SslFuncId(d, 272); }
// spDesc.pointee[23].__vftable  == byte offset 276 — "AddEnemy"
static inline int aiPLANNER_DESC_SslFnAddEnemy(const dscDESC *d)            { return aiPLANNER_DESC_SslFuncId(d, 276); }
// spDesc.pointee[23].dsREF_COUNT == byte offset 280 — "RemoveEnemy"
static inline int aiPLANNER_DESC_SslFnRemoveEnemy(const dscDESC *d)         { return aiPLANNER_DESC_SslFuncId(d, 280); }
// spDesc.pointee[23].pBrand     == byte offset 284 — "SetFlag" (tSetFlag)
static inline int aiPLANNER_DESC_SslFnSetFlag(const dscDESC *d)             { return aiPLANNER_DESC_SslFuncId(d, 284); }
// spDesc.pointee[24].__vftable  == byte offset 288 — "ClearFlag" (tClearFlag)
static inline int aiPLANNER_DESC_SslFnClearFlag(const dscDESC *d)           { return aiPLANNER_DESC_SslFuncId(d, 288); }
// spDesc.pointee[24].dsREF_COUNT == byte offset 292 — "IsFlag" (tIsFlag)
static inline int aiPLANNER_DESC_SslFnIsFlag(const dscDESC *d)              { return aiPLANNER_DESC_SslFuncId(d, 292); }
