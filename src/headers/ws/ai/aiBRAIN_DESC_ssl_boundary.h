#pragma once
// Boundary accessors into aiBRAIN's owned species descriptor (spDesc.pointee) for the aiMIND SSL
// batch. The descriptor's dynamic type is an aiBRAIN_DESC (the NEXT FRONTIER); its static type is
// dscDESC. The aiMIND SSL entry points read a handful of per-species SSL event ids and init flags
// out of it. Rather than fabricate the full aiBRAIN_DESC layout, these read the exact words the
// disassembly reads (a8_mind.cpp, 0x832291E0-0x83229AE0), documented at each site.
//
// The decompiler expresses each read as `spDesc.pointee[N].<field>` over a dscDESC[] view
// (sizeof(dscDESC) == 12); the absolute byte offsets below are N*12 + field-offset. The single-byte
// flag reads take the big-endian most-/next-significant byte of their word (HIBYTE/BYTE1/... on the
// PPC source), i.e. the byte at the lowest address of that word on the Xbox 360 descriptor image.

struct dscDESC;

// --- SSL event ids (4-byte words) ------------------------------------------------------------
// spDesc.pointee[91].refCount == byte offset 1096 — the "OnAlert" script event id.
static inline int aiBRAIN_DESC_SslEvOnAlert(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1096);
}
// spDesc.pointee[92].refCount == byte offset 1108 — the "OnInit" script event id.
static inline int aiBRAIN_DESC_SslEvOnInit(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1108);
}
// spDesc.pointee[92].pBrand == byte offset 1112 — the "NoticeWounded" script event id.
static inline int aiBRAIN_DESC_SslEvNoticeWounded(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1112);
}
// spDesc.pointee[93].__vftable == byte offset 1116 — the no-argument "OnComplete/OnDeath" event id.
static inline int aiBRAIN_DESC_SslEvOnCompleteNoArg(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1116);
}
// spDesc.pointee[93].refCount == byte offset 1120 — the "OnComplete" event id (fired with the
// killing-damage name).
static inline int aiBRAIN_DESC_SslEvOnComplete(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1120);
}
// spDesc.pointee[93].pBrand == byte offset 1124 — the "OnBattleChanged" event id.
static inline int aiBRAIN_DESC_SslEvOnBattleChanged(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1124);
}

// spDesc.pointee[91].pBrand == byte offset 1100 — the no-argument "OnReload" script event id.
static inline int aiBRAIN_DESC_SslEvOnReload(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1100);
}
// spDesc.pointee[95].pBrand == byte offset 1148 — the "OnStrikeStart" script event id (fired with
// the strike name boxed as its payload).
static inline int aiBRAIN_DESC_SslEvOnStrikeStart(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1148);
}
// spDesc.pointee[96].__vftable == byte offset 1152 — the "OnStrikeEnd" script event id (fired with
// the strike name boxed as its payload).
static inline int aiBRAIN_DESC_SslEvOnStrikeEnd(const dscDESC *desc) {
    return *reinterpret_cast<const int *>(reinterpret_cast<const unsigned char *>(desc) + 1152);
}

// --- Init flags (single descriptor bytes) ----------------------------------------------------
// spDesc.pointee[41].__vftable LOBYTE == byte offset 495 — the "use full (ammo-consuming) reload"
// flag: when set, a reload runs propWPN_AI_USABLE::Reload + OnReload; when clear, ReloadInstantly.
static inline bool aiBRAIN_DESC_UseFullReload(const dscDESC *desc) {
    return reinterpret_cast<const unsigned char *>(desc)[495] != 0;
}
// spDesc.pointee[71].pBrand HIBYTE == byte offset 860 — "force initial setup" flag.
static inline bool aiBRAIN_DESC_ForceSetupInitial(const dscDESC *desc) {
    return reinterpret_cast<const unsigned char *>(desc)[860] != 0;
}
// spDesc.pointee[58].refCount, bytes HIBYTE/BYTE1/BYTE2/LOBYTE == byte offsets 700..703 — the four
// per-species general-state init flags that seed aiSTATUS.gen bits 0x10000/0x20000/0x40000/0x80000.
static inline bool aiBRAIN_DESC_InitGenFlagA(const dscDESC *desc) {
    return reinterpret_cast<const unsigned char *>(desc)[700] != 0;
}
static inline bool aiBRAIN_DESC_InitGenFlagB(const dscDESC *desc) {
    return reinterpret_cast<const unsigned char *>(desc)[701] != 0;
}
static inline bool aiBRAIN_DESC_InitGenFlagC(const dscDESC *desc) {
    return reinterpret_cast<const unsigned char *>(desc)[702] != 0;
}
static inline bool aiBRAIN_DESC_InitGenFlagD(const dscDESC *desc) {
    return reinterpret_cast<const unsigned char *>(desc)[703] != 0;
}
