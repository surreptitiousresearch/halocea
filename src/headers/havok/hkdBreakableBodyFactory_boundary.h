#pragma once
#include "hkTransform.h"
#include "hkBool.h"

typedef struct hkdBreakableBody hkdBreakableBody;
typedef struct hkdController hkdController;
typedef struct hkpRigidBodyCinfo hkpRigidBodyCinfo;
typedef struct hkdShapeInstanceInfo hkdShapeInstanceInfo;
typedef struct hkdBreakableBodyFactory hkdBreakableBodyFactory;

/* hkdBreakableBodyFactory::CreateBodyInput, size 80 */
typedef struct hkdBreakableBodyFactory_CreateBodyInput
{
    hkTransform m_parentTransform;                 /* 0  */
    const hkdBreakableBody *m_parentBreakableBody; /* 64 */
    const hkpRigidBodyCinfo *m_templateInfo;       /* 68 */
    hkBool m_useBreakableShapeMassProperties;      /* 72 */
    unsigned char _pad0[3]; /* db-verified padding */
    hkdController *m_controller;                    /* 76 */
} hkdBreakableBodyFactory_CreateBodyInput;

hkdBreakableBody *hkdBreakableBodyFactory_createBreakableBody(
        hkdBreakableBodyFactory *factory,
        const hkdShapeInstanceInfo *instance,
        const hkdBreakableBodyFactory_CreateBodyInput *input); /* virtual createBreakableBody */
