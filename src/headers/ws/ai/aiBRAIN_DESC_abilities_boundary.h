#pragma once
// Boundary accessors into aiBRAIN's owned species descriptor (spDesc.pointee) for the two
// aiBRAIN_DESC::ABILITIES flags reached by aiWEAPON strike logic. The descriptor's dynamic type is
// an aiBRAIN_DESC (the NEXT FRONTIER); its static type is dscDESC. `abil` (aiBRAIN_DESC::ABILITIES)
// lives at descriptor offset 0x1E8; its flag bytes are absolute offsets 0x1E8 + field-offset,
// matching the disassembly at each site. Rather than fabricate the full aiBRAIN_DESC layout, these
// read the exact byte each site reads.

struct dscDESC;

// aiBRAIN_DESC::abil.NS (ABILITIES+0x01 -> byte offset 0x1E9) — nav-system feature enabled.
static inline bool aiBRAIN_DESC_AbilNS(const dscDESC *d) {
    return *(reinterpret_cast<const bool *>(reinterpret_cast<const unsigned char *>(d) + 0x1E9));
}
// aiBRAIN_DESC::abil.changeEnemyDuringStrike (ABILITIES+0x15 -> byte offset 0x1FD).
static inline bool aiBRAIN_DESC_AbilChangeEnemyDuringStrike(const dscDESC *d) {
    return *(reinterpret_cast<const bool *>(reinterpret_cast<const unsigned char *>(d) + 0x1FD));
}
// aiBRAIN_DESC::abil.checkVisForBlindShoot (ABILITIES+0x16 -> byte offset 0x1FE).
static inline bool aiBRAIN_DESC_AbilCheckVisForBlindShoot(const dscDESC *d) {
    return *(reinterpret_cast<const bool *>(reinterpret_cast<const unsigned char *>(d) + 0x1FE));
}
