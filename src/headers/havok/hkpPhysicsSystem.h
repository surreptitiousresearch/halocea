#pragma once
#include "hkReferencedObject.h"
#include "hkArray.h"

/* Element types are only ever touched as opaque hkReferencedObject pointers by
   the reversed add* methods, so they stay forward-declared. */
typedef struct hkpRigidBody hkpRigidBody;
typedef struct hkpConstraintInstance hkpConstraintInstance;
typedef struct hkpAction hkpAction;
typedef struct hkpPhantom hkpPhantom;

/* hkpPhysicsSystem, size 68 (DB types_members). */
typedef struct hkpPhysicsSystem
{
    hkReferencedObject base;                       /* 0  */
    hkArray<hkpRigidBody *> m_rigidBodies;         /* 8  */
    hkArray<hkpConstraintInstance *> m_constraints;/* 20 */
    hkArray<hkpAction *> m_actions;                /* 32 */
    hkArray<hkpPhantom *> m_phantoms;              /* 44 */
    const char *m_name;                            /* 56 */
    unsigned int m_userData;                       /* 60 */
    unsigned char m_active;                        /* 64 (hkBool) */
} hkpPhysicsSystem;

void hkpPhysicsSystem_addRigidBody(hkpPhysicsSystem *self, hkpRigidBody *body);
void hkpPhysicsSystem_addAction(hkpPhysicsSystem *self, hkpAction *action);
void hkpPhysicsSystem_addPhantom(hkpPhysicsSystem *self, hkpPhantom *phantom);
