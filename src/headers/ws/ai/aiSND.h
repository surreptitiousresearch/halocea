#pragma once
#include "aiSND_IFACE.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08: the concrete per-brain sound module (aiMIND::snd pointee). DB-verified layout
// (types_members aiSND) — size 16: aiSND_IFACE base@0 (8), permissions@8, voiceType@0xC.

// ?AI_SNDST — sound-module permission bitflags (DB types_enum_values AI_SNDST).
enum AI_SNDST {
    AI_SNDP_ENABLE_SCRIPTEDVOS   = 1,
    AI_SNDP_ENABLE_EXCLAMATIONS  = 2,
    AI_SNDP_ENABLE_PITCH         = 4,
    AI_SNDP_LASTSTATE            = 5,
};

struct aiBRAIN;

struct aiSND : aiSND_IFACE {
    dsFLAGS<AI_SNDST, int> permissions; // 0x08 which sound behaviours are currently allowed
    dsSTRID                voiceType;   // 0x0C resolved voice-type id ("mute" clears exclamations)

    // 0x832989D0 (??0aiSND@@QAA@PAVaiBRAIN@@@Z) — construct bound to owning brain.
    aiSND(aiBRAIN *);

    // 0x832997A8 (?ProcessInit@aiSND@@UAAXXZ) — virtual: per-spawn init. Resolves the voice type from
    // the brain descriptor and enables the pitch response only when the pitch curve has real keys.
    void ProcessInit();

    // 0x83299310 (?SetVoiceType@aiSND@@IAAXABV?$dsTSTRING@D@@@Z) — protected: adopt `name` as the voice
    // type id; an empty or "mute" name disables exclamations instead.
    void SetVoiceType(const dsTSTRING<char> &name);

    // 0x832999F8 (?ProcessFrame@aiSND@@UAAXXZ) — virtual: per-frame sound update (times ProcessPitch
    // under a named profiling counter). REVERSED: aiSND__ProcessFrame.cpp.
    void ProcessFrame();

    // 0x832996B0 (?ProcessPitch@aiSND@@IAAXXZ) — protected: when pitch is enabled, push the unit's
    // current speed through the descriptor's pitch curve into the RPC-marked playing sound.
    void ProcessPitch();

    // 0x832993B8 (?IsExclamationAccepted@aiSND@@UAA_NABVdsSTRID@@@Z) — virtual: whether exclamation
    // `name` may play now (dead brains defer to the planner's BASS accept-for-dead rule).
    bool IsExclamationAccepted(const dsSTRID &name);

    // 0x83299828 (?BuildExclamationName@aiSND@@UAA?AVdsSTRID@@ABV2@ABV?$dsTSTRING@D@@1@Z) — virtual:
    // compose "<prefix><voiceType>_<name><postfix>" and intern it as a dsSTRID (returned by value).
    dsSTRID BuildExclamationName(const dsSTRID &name, const dsTSTRING<char> &prefix,
                                 const dsTSTRING<char> &postfix);
};
