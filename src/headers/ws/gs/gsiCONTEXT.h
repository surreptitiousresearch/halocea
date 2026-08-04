#pragma once
// ws-engine gs: an input-binding context (e.g. "debug", "general") that owns a set of
// action mappings, referenced via ds_data::REF_TYPE<gsiCONTEXT> handles held by
// gsINP_SYSTEM. DB-verified layouts (types_members); canonical home of the
// input-context aggregate block (haloENGINE_CONTROL_boundary.h includes this).

#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/MAP.h"

struct gsINPUT_DEVICE; // boundary — physical input device
struct inpDEVICE;      // boundary
struct gsiCOMMAND;     // boundary — bound input command

// types_members gsINPUT_LAYOUT: name@0 (dsSTRID), cmdInfoList@4 — size 24.
typedef struct gsINPUT_LAYOUT {
    struct CMD_INFO; // boundary — per-command binding record (referenced only via the vector)
    dsSTRID                                name;        // 0x00
    dsVECTOR<gsINPUT_LAYOUT::CMD_INFO, 8>  cmdInfoList; // 0x04
} gsINPUT_LAYOUT;

// types_members gsiFORCE_FEEDBACK: isEnabled@0, isPaused@1, effects@4, devices@24 — size 44.
typedef struct gsiFORCE_FEEDBACK {
    bool                       isEnabled;  // 0x00
    bool                       isPaused;   // 0x01
    unsigned char              _pad02[2];  // 0x02
    dsVECTOR<dsSTRID, 8>       effects;    // 0x04
    dsVECTOR<inpDEVICE *, 8>   devices;    // 0x18
} gsiFORCE_FEEDBACK;

// types_members gsiCONTEXT: cmdMap@0, name@84, priority@88, deviceInfo@92, focusDevice@112,
// isActive@116, isInited@117, forceFeedback@120 — size 164.
typedef struct gsiCONTEXT {
    // types_members gsiCONTEXT::DEVICE_INFO: device@0, layout@4 — size 28.
    struct DEVICE_INFO {
        gsINPUT_DEVICE *device; // 0x00
        gsINPUT_LAYOUT  layout; // 0x04
    };

    ds::MAP<dsSTRID, gsiCOMMAND *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> cmdMap; // 0x00
    dsSTRID                                 name;          // 0x54
    int                                     priority;      // 0x58
    dsVECTOR<gsiCONTEXT::DEVICE_INFO, 8>    deviceInfo;    // 0x5C
    dsSTRID                                 focusDevice;   // 0x70
    bool                                    isActive;      // 0x74
    bool                                    isInited;      // 0x75
    unsigned char                           _pad76[2];     // 0x76
    gsiFORCE_FEEDBACK                       forceFeedback; // 0x78 — size 164

    // ?Activate@gsiCONTEXT@@QAAX_N@Z — enable/disable this binding context.
    void Activate(bool isActivate); // boundary

    // 0x823CDAB0 — convenience overload: intern `cmd` into a dsSTRID and forward to the
    // dsSTRID overload. Reversed in gsiCONTEXT__IsJustOn.cpp.
    bool IsJustOn(const char *cmd);
    // 0x82xxxxxx — true if the named command transitioned to "on" this frame. boundary
    // (the dsSTRID overload the char* one forwards to).
    bool IsJustOn(const dsSTRID &cmd);
} gsiCONTEXT;
