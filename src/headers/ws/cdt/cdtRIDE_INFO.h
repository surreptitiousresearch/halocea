#pragma once
#include "../ent/entTRACKER.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dV.h"
// ws-engine cdt: ride-watcher state — tracks the object a body is standing/riding on so its
// motion can be composed into the rider's transform. DB-verified layout (types_members
// cdtRIDE_INFO): isConstraintOrientUp@0, tracker@4 (entTRACKER, 96), isLocked@100,
// matrObjPrev@104 (m3dMATR, 64), elapsedTimePrev@168, ridePositionWCS@172 (m3dMATR, 64),
// isPhys@236, isRideSet@237, rideSpeed@240 (m3dV) — size 252.

struct cdtRIDE_INFO {
    bool          isConstraintOrientUp; // 0x00
    unsigned char _pad01[3];            // 0x01
    entTRACKER    tracker;              // 0x04 tracked ride object
    bool          isLocked;             // 0x64
    unsigned char _pad65[3];            // 0x65
    m3dMATR       matrObjPrev;          // 0x68 ride object's previous-frame matrix
    float         elapsedTimePrev;      // 0xA8
    m3dMATR       ridePositionWCS;      // 0xAC rider's world-space ride anchor
    bool          isPhys;               // 0xEC
    bool          isRideSet;            // 0xED
    unsigned char _padEE[2];            // 0xEE
    m3dV          rideSpeed;            // 0xF0
};                                      // 252 bytes
