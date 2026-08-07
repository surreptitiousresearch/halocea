#pragma once
#include "../ia/iaIACTOR.h"
#include "../msg/msgRES.h"
#include "../anim/animINST.h"
#include "entSFX_LIST.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../ds/MAP.h"
#include "entFOLLOW.h"
// ws-engine ent subsystem: root game-entity class.
// DB-verified layout (types_members entENTITY) — total size 444 (0x1BC):
//   iaIACTOR@0 (200B, base), followObj@200 (entFOLLOW, 112B), sfxList@312 (entSFX_LIST, 28B),
//   pDomSpawn@340 (gsDOM_SPAWN*, 4B), stateEnt@344 (dsFLAGS<ENT_ST,int>, 4B),
//   pInst@348 (animINST*, 4B), pFloorInfo@352 (propFLOOR_INFO*, 4B),
//   cnt@356 (apCOUNTER_TIME_CALL*, 4B), farmCounters@360
//   (ds::MAP<dsTSTRING<char>,int,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>, 84B).
// The pDomSpawn/pFloorInfo/cnt pointee classes are OTHER-SUBSYSTEM BOUNDARIES
// (forward-declared pointers);
// sfxList, stateEnt, and pInst are the fields the ent batch (GetNumSfx/OnRegister/Register)
// actually touches and are fully typed.

#include "ENT_ST.h" // ent subsystem — entENTITY state-flag enum (DB types_enum_values)
struct gsDOM_SPAWN;       // gs subsystem — dominant-spawn info (pointer only)                boundary
struct propFLOOR_INFO;    // prop subsystem — floor-standing info (pointer only)              boundary
struct apCOUNTER_TIME_CALL; // ap subsystem — per-entity timing counter (pointer only)        boundary
struct entCREATE_INFO;    // entCREATE_INFO.h — entity creation/registration parameter block  boundary (fwd)
struct fioFILE;           // fio/fioFILE.h — chunked save stream (pointer only)               boundary (fwd)

struct entENTITY : iaIACTOR {
    entFOLLOW     followObj;       // 0xC8 (200, 112B) — follow-hierarchy link block (entFOLLOW.h)
    entSFX_LIST   sfxList;         // 0x138 (312) — sound-effect list; GetNumSfx returns sfxList.sfxArray.nElem
    gsDOM_SPAWN  *pDomSpawn;       // 0x154 (340) — boundary pointer
    dsFLAGS<ENT_ST, int> stateEnt; // 0x158 (344) — entity state bitflags (Register sets bit 0x1 when rendering)
    animINST     *pInst;           // 0x15C (348) — animated instance owned by this entity
    propFLOOR_INFO *pFloorInfo;    // 0x160 (352) — boundary pointer
    apCOUNTER_TIME_CALL *cnt;      // 0x164 (356) — boundary pointer
    ds::MAP<dsTSTRING<char>, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> farmCounters; // 0x168 (360, 84B)

    // Virtual: per-spawn entity init (base of aiBRAIN::ProcessINIT). Returns nonzero on success.
    // boundary — body external to this batch.
    int ProcessINIT(void *pInfo);

    // 0x827068A8 — number of live sound-effect instances tracked by this entity.
    int GetNumSfx() const;

    // 0x8270BD80 — bounds-checked accessor for the idx-th live sound-effect instance; nullptr when
    // idx is out of range.
    struct entSFX *GetSfx(int idx) const;
    // ?GetSfx@entENTITY@@QBAPAVentSFX@@PBD@Z — the sound-effect instance named `name` (null if none).
    // boundary — body external to this batch.
    struct entSFX *GetSfx(const char *name) const;
    // ?implStartSfx@entENTITY@@UAAXABV?$dsTSTRING@D@@@Z @0x8269FDB8 — virtual: start the named sfx.
    // boundary — body external to this batch.
    void implStartSfx(const dsTSTRING<char> &name);
    // ?implStopSfx@entENTITY@@UAAXABV?$dsTSTRING@D@@@Z @0x8269FE88 — virtual: stop the named sfx.
    // boundary — body external to this batch.
    void implStopSfx(const dsTSTRING<char> &name);

    // ?ProcessFRAME@entENTITY@@MAAXXZ @ 0x82536F40 — base per-frame entity update; invoked
    // (non-virtually, as a base call) by derived ProcessFRAME overrides (e.g. aiSQUAD::ProcessFRAME).
    // Declared non-virtual to avoid claiming an unresolved vtable slot; body external.  boundary
    void ProcessFRAME();
    // ?ProcessMsg@entENTITY@@UAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z @ 0x82532898 — base message dispatch;
    // invoked as a base call by derived ProcessMsg-style overrides (e.g. aiSQUAD::ProcessMSG_INIT).
    // Declared non-virtual here (base-call site) to avoid claiming an unresolved vtable slot.  boundary
    msgRES ProcessMsg(int msg, void *pInfo, struct msgADDR *pSend);

    // 0x82534618 — display name: from pInst->name when an instance exists, else a
    // refcount-shared copy of iaIACTOR::nameIActive. Returned by value.
    dsTSTRING<char> GetName() const;

    // 0x82536068 — main registration driver: seeds render-state, notifies the level system
    // around the base msgADDR init message, and (first call only) bumps an instrumentation
    // counter. Returns whether registration succeeded. Protected (IAA mangle).
    bool Register(const entCREATE_INFO &info);

    // vtable slot +0x174 (UAA mangle: `?implSetMaterial@entENTITY@@UAAXABV?$dsTSTRING@D@@0@Z`) —
    // apply a named material override (`mtlName`, param1) to the object subtree selected by
    // `objName` (param2; empty = whole model — disasm @0x826A00F8 empty-checks param2 then walks
    // objITER_TREE). Param roles disasm-confirmed 2026-08-04; reversed in
    // src/ws/ent/entENTITY__implSetMaterial.cpp.
    void implSetMaterial(const dsTSTRING<char> &mtlName, const dsTSTRING<char> &objName);

    // Virtual termination hook — base entity teardown, invoked by derived overrides (e.g.
    // aiBRAIN::ProcessTERM calls entENTITY::ProcessTERM on its base subobject). Body external. boundary.
    void ProcessTERM();

    // 0x826A88B0 / 0x826A8A80 — hide/show this entity: flip the sml SHOW/HIDE/START states, mark
    // the model instance hidden (pInst->state bit 0), pause/resume+hide/show the sfx list and the
    // playing-sound list, and propagate to the property container. implHide latches an internal
    // 0x1000 "hidden" flag (guarding re-entry) plus a 0x2000000 "was-active" flag.
    void implHide();
    void implShow();

    // entENTITY::Hide / entENTITY::Show (_Hide_entENTITY__QAAXXZ / _Show_entENTITY__QAAXXZ) —
    // public visibility toggles that drive implHide/implShow (plus the follow-hierarchy). boundary.
    void Hide();
    void Show();

    // 0x8252FD88 / 0x8252FC68 — per-camera visibility: toggle the model instance's state2 camera
    // bits (0x80 for camera 0, 0x100 for camera 1), forward to every sfx, then to the property
    // container. idCamera selects the camera slot.
    void ShowToCamera(int idCamera);
    void HideFromCamera(int idCamera);

    // Virtual — write the entity's world-space centre into `out`. Dispatched by
    // aiPLANNER::NoticeGrenadeInit on the grenade entity.  boundary — body external to this batch.
    void GetCenter(m3dV *out);

    // Virtual (entENTITY_vtbl+0x198) — write the entity's world-space velocity into `out`.
    // Reached by aiWATCHER::GetSpeed.  boundary — body external to this batch.
    void GetSpeed(m3dV *out);

    // --- base virtuals invoked (as base calls) by aiBRAIN's overrides. Bodies external. boundary ---
    // 0x8252F710 (?PreProcessMsg@entENTITY@@UAAHHPAXPAVmsgADDR@@@Z) — base pre-dispatch message hook.
    int  PreProcessMsg(int msg, void *pInfo, struct msgADDR *pSend);
    // 0x82538C18 / 0x8253ABE0 — base save-data (farm-counters) serialization.
    void WriteSaveDataFarm(struct fioFILE *stream);
    void ReadSaveDataFarm(struct fioFILE *stream);
    // 0x8269EE00 (?implSetLOD@entENTITY@@UAAXH@Z) — base LOD setter.
    void implSetLOD(int idx);

private:
    // 0x8252F910 — registration hook: adopts a caller-supplied multiplayer unique id, if any.
    void OnRegister(const entCREATE_INFO &info);
};
