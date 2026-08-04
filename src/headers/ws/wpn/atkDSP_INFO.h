#pragma once
// ws-engine wpn/atk: attack-dispatch info block filled by wpnWEAPON::GetAttackInfo describing a
// given attack — validity flags plus projectile launch speed/acceleration.
// DB-verified layout (types_members atkDSP_INFO): a packed flag byte at offset 0 (2 reserved bits
// then isFlyThrough/isPhysAtk/isPjlAtk/isInstantAtk/isAccelValid/isSpeedValid), speed@4, accel@8.
// Reproduced faithfully; callers gate on the raw flag byte (bit mapping preserved from the binary).

struct atkDSP_INFO {
    unsigned char _reserved   : 2; // 0x00 bits
    unsigned char isFlyThrough : 1;
    unsigned char isPhysAtk    : 1;
    unsigned char isPjlAtk     : 1;
    unsigned char isInstantAtk : 1;
    unsigned char isAccelValid : 1;
    unsigned char isSpeedValid : 1;
    unsigned char _pad01[3];
    float         speed;           // 0x04 projectile launch speed
    float         accel;           // 0x08 projectile acceleration (gravity)

    // ?atkDSP_INFO@atkDSP_INFO@... default ctor — zero-init. boundary (body external to this batch).
    atkDSP_INFO();
};
