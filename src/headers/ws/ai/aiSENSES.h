#pragma once
// ws-engine ai08: a single perception-range descriptor (sight cone half-angles + hearing radii).
// DB-verified layout (types_members aiSENSES) — size 0x1C.

struct aiSENSES {
    float angleSeeStraight; // 0x00 forward sight cone half-angle (rad)
    float angleSeeSide;     // 0x04 peripheral sight cone half-angle (rad)
    float radSeeStraight;   // 0x08 forward sight range
    float radSeeSide;       // 0x0C peripheral sight range
    float radHearShot;      // 0x10 gunshot hearing radius
    float radHearRun;       // 0x14 running-footstep hearing radius
    float radHearWalk;      // 0x18 walking-footstep hearing radius
};
