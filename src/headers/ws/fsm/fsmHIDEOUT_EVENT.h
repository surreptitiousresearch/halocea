#pragma once
#include "fsmEVENT_BASE.h"
#include "fsmAIMING_EVENT.h"        // fsmEVENT_PARAM_BASE (single canonical definition)
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSHARED_PTR.h"

// ws-engine fsm: the "hideout" body-FSM event family reached by aiHO_SYS (DoChangeHO / SendEnterHOEvent
// / SendLeaveHOEvent). Only the change-side event id and the shared PARAM block are modeled here; the
// remaining event ids (enter_/enter_stand_/enter_back_/...) are the next frontier.  boundary.

struct mdlITRC_BASE; // mdl — interactive transform ref component (mdlITRC_BASE.h)

// DB types_members fsmHIDEOUT_EVENT: one row — the empty fsmEVENT_BASE base at offset 0.
struct fsmHIDEOUT_EVENT : fsmEVENT_BASE {
    // ?change_side_@fsmHIDEOUT_EVENT@@SAABVdsSTRID@@XZ @ 0x829BE420 — static: interned "change_side" id.
    static const dsSTRID &change_side_();

    // ?leave_@fsmHIDEOUT_EVENT@@SAABVdsSTRID@@XZ — static: interned "leave" hideout event id. boundary.
    static const dsSTRID &leave_();

    // DB-verified layout (types_members fsmHIDEOUT_EVENT::PARAM): fsmEVENT_PARAM_BASE@0,
    // itrcList@0 (dsVECTOR, 20B), itrcPrev@0x14 (dsSHARED_PTR, 8B), then six flag bytes.
    struct PARAM : fsmEVENT_PARAM_BASE {
        dsVECTOR<dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> >, 8> itrcList; // 0x00
        dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> >             itrcPrev; // 0x14
        bool isFast;           // 0x1C
        bool isLeaveWln;       // 0x1D
        bool isEnterWln;       // 0x1E
        bool isThroughSmart;   // 0x1F
        bool isNeedShake;      // 0x20
        bool isAppendListItrc; // 0x21

        // ??0PARAM@fsmHIDEOUT_EVENT@@QAA@ABV?$dsSHARED_PTR@VmdlITRC_BASE@@...@@@Z — construct from the
        // previous ITRC handle (records it in itrcPrev). Body external to this batch. boundary.
        PARAM(const dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > &itrcPrev);
    };
};
