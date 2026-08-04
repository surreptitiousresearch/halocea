#pragma once
#include "../dsc/dscDESC.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "aiSENSES.h"

// Boundary accessors into aiBRAIN's owned species descriptor (spDesc.pointee) for the perception
// module (aiPERC / aiISA / aiBRAIN::implSetSenses). The full descriptor is an aiBRAIN_DESC; the
// decompiler expresses each read as a raw byte view over a dscDESC[] pointer. These helpers name
// the reached byte offsets (DB-verified against the ai08 perception disassembly) so the reversed
// TUs read the descriptor without re-modelling the whole (next-frontier) aiBRAIN_DESC layout.
// Same established pattern as aiBRAIN_DESC_sslfunc_boundary.h / aiBRAIN_DESC_ssl_boundary.h.

static inline const unsigned char *aiBRAIN_DESC_Bytes(const dscDESC *desc) {
    return reinterpret_cast<const unsigned char *>(desc);
}
static inline float aiBRAIN_DESC_PercFloat(const dscDESC *desc, unsigned int byteOffset) {
    return *reinterpret_cast<const float *>(aiBRAIN_DESC_Bytes(desc) + byteOffset);
}
static inline const dsVECTOR<dsSTRID, 8> *aiBRAIN_DESC_PercPool(const dscDESC *desc, unsigned int byteOffset) {
    return reinterpret_cast<const dsVECTOR<dsSTRID, 8> *>(aiBRAIN_DESC_Bytes(desc) + byteOffset);
}
static inline const aiSENSES *aiBRAIN_DESC_SensesPreset(const dscDESC *desc, unsigned int byteOffset) {
    return reinterpret_cast<const aiSENSES *>(aiBRAIN_DESC_Bytes(desc) + byteOffset);
}

// --- fly-object (grenade / physics-object) reaction tuning (aiPERC::ChoosePool/NoticeFlyObject) ---
// byte 0x3C0 (960) — minimum reaction distance / below-band threshold.
static inline float aiBRAIN_DESC_FlyObjReactDistMin(const dscDESC *d) { return aiBRAIN_DESC_PercFloat(d, 0x3C0); }
// byte 0x3EC (1004) — maximum reaction distance (top of the react band).
static inline float aiBRAIN_DESC_FlyObjReactDistMax(const dscDESC *d) { return aiBRAIN_DESC_PercFloat(d, 0x3EC); }
// The four candidate FSM-event pools (dsVECTOR<dsSTRID,8>) selected by band + line-of-fire + side.
static inline const dsVECTOR<dsSTRID, 8> *aiBRAIN_DESC_FlyObjReactNearRight(const dscDESC *d) { return aiBRAIN_DESC_PercPool(d, 0x3C4); } // 964
static inline const dsVECTOR<dsSTRID, 8> *aiBRAIN_DESC_FlyObjReactNearLeft (const dscDESC *d) { return aiBRAIN_DESC_PercPool(d, 0x3D8); } // 984 (dscDESC[82])
static inline const dsVECTOR<dsSTRID, 8> *aiBRAIN_DESC_FlyObjReactFarRight (const dscDESC *d) { return aiBRAIN_DESC_PercPool(d, 0x3F0); } // 1008 (dscDESC[84])
static inline const dsVECTOR<dsSTRID, 8> *aiBRAIN_DESC_FlyObjReactFarLeft  (const dscDESC *d) { return aiBRAIN_DESC_PercPool(d, 0x404); } // 1028

// --- infliction / injury tuning (aiPERC::UpdateTimers/UpdateInjury) ---
// byte 0x264 (612) — damage-accumulation rate multiplier applied to dt.
static inline float aiBRAIN_DESC_InflictionDmgRate(const dscDESC *d) { return aiBRAIN_DESC_PercFloat(d, 0x264); }
// byte 0x268 (616) — heavy-damage reaction threshold (2000000.0 sentinel = disabled).
static inline float aiBRAIN_DESC_HeavyDamageThreshold(const dscDESC *d) { return aiBRAIN_DESC_PercFloat(d, 0x268); }
// byte 0x274 (628) — injury sub-block update parameter (wound decay).
static inline float aiBRAIN_DESC_InjuryUpdateParam(const dscDESC *d) { return aiBRAIN_DESC_PercFloat(d, 0x274); }

// --- senses presets copied into the status block by aiBRAIN::implSetSenses ---
// byte 0x210 (528) — idle senses preset (7 floats == aiSENSES).
static inline const aiSENSES *aiBRAIN_DESC_IdleSensesPreset(const dscDESC *d)   { return aiBRAIN_DESC_SensesPreset(d, 0x210); }
// byte 0x22C (556) — battle/default senses preset (7 floats == aiSENSES).
static inline const aiSENSES *aiBRAIN_DESC_BattleSensesPreset(const dscDESC *d) { return aiBRAIN_DESC_SensesPreset(d, 0x22C); }
