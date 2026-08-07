#pragma once
#include "fsmEVENT_BASE.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dMATR.h"
// ws-engine fsm: the "aiming" body-FSM event family reached by aiBRAIN::implSetInterest /
// implEndInterest. Only the two event-id accessors and the look-at parameter block are modeled here;
// the rest of the family is the next frontier.  boundary.

// Empty base of every FSM event parameter block (DB fsmEVENT_PARAM_BASE: no data members).
struct fsmEVENT_PARAM_BASE {};

// DB types_members fsmAIMING_EVENT: one row — the empty fsmEVENT_BASE base at offset 0.
struct fsmAIMING_EVENT : fsmEVENT_BASE {
    // ?look_at_@fsmAIMING_EVENT@@SAABVdsSTRID@@XZ — static: the interned "look_at" event id.  boundary.
    static const dsSTRID &look_at_();
    // ?off_look_at_@fsmAIMING_EVENT@@SAABVdsSTRID@@XZ — static: the interned "off_look_at" id.  boundary.
    static const dsSTRID &off_look_at_();
    // ?set_@fsmAIMING_EVENT@@SAABVdsSTRID@@XZ — static: the interned "set" (aim direction) id.  boundary.
    static const dsSTRID &set_();

    // DB-verified layout (types_members fsmAIMING_EVENT::LOOK_AT_PARAM): fsmEVENT_PARAM_BASE@0,
    // mode@0 (dsTSTRING), trk@4 (dsTSTRING), isIdlingDisabled@8, isAccepted@9.
    struct LOOK_AT_PARAM : fsmEVENT_PARAM_BASE {
        dsTSTRING<char> mode;             // 0x00 look-at mode name
        dsTSTRING<char> trk;              // 0x04 tracker name to look at
        bool            isIdlingDisabled; // 0x08 in: suppress idling while looking
        bool            isAccepted;       // 0x09 out: whether the FSM accepted the look-at
    };

    // "set" aiming-direction callback param block filled by aiBODY::SendFsmEventPosAim: aim position,
    // aim direction (zeroed), a full transform (identity), and two trailing flags. Layout matches the
    // buffer built at the call site (pos@0, dir@0xC, matr@0x18, flags@0x58/0x59).  boundary.
    struct SET_PARAM {
        m3dV    pos;   // 0x00 aim position
        m3dV    dir;   // 0x0C aim direction (zero at the call site)
        m3dMATR matr;  // 0x18 aim transform (identity at the call site)
        bool    flag0; // 0x58
        bool    flag1; // 0x59
    };
};
