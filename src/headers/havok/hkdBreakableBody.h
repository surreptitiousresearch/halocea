#pragma once
#include "hkEnum.h"
#include "hkBool.h"
#include "hkRefPtr.h"
#include "hkSmallArray.h"
#include "hkArray.h"
#include "hkReferencedObject.h"

typedef struct hkdBreakableShape hkdBreakableShape;
typedef struct hkdController hkdController;
typedef struct hkdWorld hkdWorld;
typedef struct hkpRigidBody hkpRigidBody;
typedef struct hkdIntegritySystem hkdIntegritySystem;
typedef struct hkdGraphicsBody hkdGraphicsBody;
typedef struct hkpProperty hkpProperty;
typedef struct hkdBreakableBodyListener hkdBreakableBodyListener;
typedef struct hkdBreakableShapeFixedConnectivity hkdBreakableShapeFixedConnectivity;

/* types_members hkpBreakableBody (size 8) — just the hkReferencedObject base. */
struct hkpBreakableBody : hkReferencedObject
{
};
typedef struct hkpBreakableBody hkpBreakableBody;

/* types_enum_values hkdBreakableBody::BodyType */
enum BodyType
{
    BODY_TYPE_INVALID = 0,
    BODY_TYPE_SIMPLE = 1,
    BODY_TYPE_EMBEDDED = 2,
    BODY_TYPE_NUM_TYPES = 3,
};

/* types_members hkdProperties (size 12) — single definition lives in hkdProperties.h
   (was duplicated here, causing C2011 when both headers were included in one TU). */
#include "hkdProperties.h"

/* types_members hkdBreakableBody (size 68) */
struct hkdBreakableBody : hkpBreakableBody
{
    hkEnum<BodyType, unsigned char> m_type;                          /* 0x08 */
    hkBool m_attachToNearbyObjects;                                  /* 0x09 */
    unsigned char _pad0A[2];                                         /* 0x0A */
    hkdWorld *m_destructionWorld;                                    /* 0x0C */
    hkRefPtr<hkpRigidBody> m_physicsBody;                            /* 0x10 */
    hkRefPtr<const hkdBreakableShape> m_breakableShape;              /* 0x14 */
    hkRefPtr<hkdController> m_controller;                            /* 0x18 */
    hkRefPtr<hkdIntegritySystem> m_integritySystem;                  /* 0x1C */
    unsigned int m_ancesterIntegrityUid;                             /* 0x20 */
    float m_constraintStrength;                                      /* 0x24 */
    hkRefPtr<hkdGraphicsBody> m_graphicsBody;                        /* 0x28 */
    hkdProperties m_properties;                                      /* 0x2C */
    hkSmallArray<hkdBreakableBodyListener *> m_listeners;            /* 0x38 */
    hkRefPtr<hkdBreakableShapeFixedConnectivity> m_fixedConnectivity;/* 0x40 */
};
typedef struct hkdBreakableBody hkdBreakableBody;
