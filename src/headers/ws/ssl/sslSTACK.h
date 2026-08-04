#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsSTRID.h"
#include "sslSTACK_DATA.h"
#include "sslSTACK_STATE.h"
#include "sslFOREACH_DATA.h"
// ssl subsystem: the SSL script VM evaluation/local/foreach stack. DB-verified layout
// (types_members sslSTACK): mainStack@0, localsStack@20, foreachStack@40, namedParams@60 —
// size 80. Each member is a dsVECTOR<T,8> (12-byte header + 8-element inline store => 20 bytes).

typedef struct sslSTACK {
    // Local-variable record (element of localsStack). DB-verified (types_members
    // sslSTACK::LOCAL_VAR): pType@0, val@4 (dsDATA) — size 12.
    struct LOCAL_VAR {
        const dsDATA_TYPE *pType; // 0x00 declared variable type
        dsDATA             val;   // 0x04 current type-erased value
    };

    dsVECTOR<sslSTACK_DATA, 8>  mainStack;    // 0x00 operand/evaluation stack
    dsVECTOR<LOCAL_VAR, 8>      localsStack;  // 0x14 local-variable stack
    dsVECTOR<sslFOREACH_DATA, 8> foreachStack; // 0x28 active foreach-loop cursors
    dsVECTOR<dsSTRID, 8>        namedParams;  // 0x3C named-parameter name stack

    // 0x82AF1AB8 — reference to the top operand-stack entry, or a shared default (TYPE_INT / 0)
    // when the operand stack is empty.
    sslSTACK_DATA &GetTop();
    // 0x82AF1BC0 — reference to local `idx`'s value, or a shared empty dsDATA when out of range.
    const dsDATA &GetLocalVar(int idx);
    // 0x82AF4590 — ensure local `idx` exists, set its declared type (defaulting to the null type),
    // and reset its value to a freshly-constructed default.
    void ResetLocalVar(const dsDATA_TYPE *pType, int idx);
    // 0x82AF1B40 — reference to operand-stack entry `idx`.
    sslSTACK_DATA &At(int idx);
    // 0x82AF1B48 — drop the top operand-stack entry (no-op if empty).
    void Pop();
    // 0x82AF0588 — number of live locals (base offset for a new frame's locals).
    int GetLocalVarsOfs();
    // 0x82AF1EF0 — reference to the innermost active foreach cursor.
    sslFOREACH_DATA &GetTopForEach();
    // 0x82AF1ED0 — drop the innermost foreach cursor (no-op if none active).
    void FreeTopForeach();
    // 0x82AF0560 — snapshot the current live-element counts of all four sub-stacks into `state`.
    void GetState(sslSTACK_STATE &state);
    // 0x82AF4508 — append a fresh zero-initialised (TYPE_INT/0) operand and return the new slot.
    sslSTACK_DATA &Push();
    // 0x82AF1B60 — truncate all four sub-stacks back to the live counts recorded in `state`.
    void RestoreState(const sslSTACK_STATE &state);
    // 0x82AF4670 — push a new foreach cursor filtered by class `cls`, with lastObjIdx = -1.
    void AllocForeach(sslCLASS_REF cls);

    // 0x82AF1C58 — store `val` into local variable `idx`: type-check/convert to the local's declared
    // type when set, else adopt the value directly. Returns sslERROR by value (sret): a no-error
    // result on success, an SSL_ERR_RUNTIME error on an out-of-range index or a failed conversion.
    sslERROR SetLocalVar(int idx, const dsDATA &val);
} sslSTACK;
