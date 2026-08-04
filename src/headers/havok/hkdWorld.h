#pragma once
#include "hkReferencedObject.h"
#include "hkRefPtr.h"
#include "hkArray.h"
#include "hkSmallArray.h"
#include "hkPointerMap.h"

typedef struct hkdBreakableBodyFactory hkdBreakableBodyFactory;
typedef struct hkdController hkdController;
typedef struct hkpWorld hkpWorld;
typedef struct hkdGraphicsSystem hkdGraphicsSystem;
typedef struct hkdPhysicsBridge hkdPhysicsBridge;
typedef struct hkpBreakOffPartsUtil hkpBreakOffPartsUtil;
typedef struct hkdConstraintBridge hkdConstraintBridge;
typedef struct hkdBreakableBody hkdBreakableBody;
typedef struct hkdIntegritySystem hkdIntegritySystem;
typedef struct hkdBreakableBodyListener hkdBreakableBodyListener;
typedef struct hkdConstraintReattachUtil hkdConstraintReattachUtil;
typedef struct hkdAddRemoveBodyForwarder hkdAddRemoveBodyForwarder;

/* hkdWorld — Havok Destruction world (DB types_members hkdWorld, 88 bytes). */
typedef struct hkdWorld
{
    hkReferencedObject base;                                    /* 0  */
    hkRefPtr<hkpWorld> m_physicsWorld;                          /* 8  */
    hkRefPtr<hkdGraphicsSystem> m_graphicsSystem;               /* 12 */
    hkRefPtr<hkdController> m_controller;                       /* 16 */
    hkRefPtr<hkdBreakableBodyFactory> m_breakableBodyFactory;   /* 20 */
    hkRefPtr<hkdPhysicsBridge> m_physicsBridge;                 /* 24 */
    hkRefPtr<hkpBreakOffPartsUtil> m_breakOffPartsUtil;         /* 28 */
    hkRefPtr<hkdConstraintBridge> m_breakableConstraintBridge;  /* 32 */
    hkPointerMap<hkdBreakableBody *,void *> m_breakableBodies;  /* 36 */
    hkArray<hkdIntegritySystem *> m_integritySystems;           /* 48 */
    hkSmallArray<hkdBreakableBodyListener *> m_listeners;       /* 60 */
    int m_numFramesExtraIntegrityConstraints;                   /* 68 */
    hkdConstraintReattachUtil *m_constraintReattachUtil;        /* 72 */
    hkdAddRemoveBodyForwarder *m_addRemoveBodyForwarder;        /* 76 */
    int m_frameCounter;                                         /* 80 */
    int m_integritySystemsUid;                                  /* 84 */
} hkdWorld; /* 88 bytes */
