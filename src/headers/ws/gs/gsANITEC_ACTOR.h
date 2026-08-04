#pragma once
#include "../ent/entENTITY.h"   // base; provides iaIACTOR::sslObject (sslOBJ_REF @ 0x58)
#include "../ent/entDESC.h"     // base of gsANITEC_ACTOR_DESC
// ws-engine gs: the SSL-scripted cinematic-actor entity. Derives from entENTITY (types_members
// gsANITEC_ACTOR: single base entENTITY, 444 bytes). Its per-type descriptor (gsANITEC_ACTOR_DESC)
// caches the resolved SSL function/event ids used to drive the cinematic from script.

// DB-verified layout (types_members gsANITEC_ACTOR_DESC): base entDESC@0 (440B),
// isSslClassInited@440 (bool), sslFuncId_StartCine@444, sslFuncId_AbortCine@448,
// sslEventId_OnCineInterrupted@452, sslFuncId_ShakeCamera@456 — the resolved script entry ids.
struct gsANITEC_ACTOR_DESC : public entDESC {
    bool isSslClassInited;               // 0x1B8 (440)
    unsigned char _pad1B9[3];            // 0x1B9 db-verified padding
    int  sslFuncId_StartCine;            // 0x1BC (444)
    int  sslFuncId_AbortCine;            // 0x1C0 (448)
    int  sslEventId_OnCineInterrupted;   // 0x1C4 (452)
    int  sslFuncId_ShakeCamera;          // 0x1C8 (456)
};

struct gsANITEC_ACTOR : public entENTITY {
    // 0x825B9880 — fire the SSL "start cinematic" function on this actor's script object.
    // Reversed in gsANITEC_ACTOR__StartCine.cpp.
    void StartCine();
};

// Per-entity descriptor accessor (entity-system template): Desc<T>(self) returns the entity's
// cached descriptor. The descriptor type is selected per entity class via ENTITY_DESC_OF.
// boundary — instantiated by the entity framework, not reversed here.
template<class T> struct ENTITY_DESC_OF;
template<> struct ENTITY_DESC_OF<gsANITEC_ACTOR> { typedef gsANITEC_ACTOR_DESC type; };

template<class T>
typename ENTITY_DESC_OF<T>::type *Desc(T *self);
