#pragma once
#include "hkReferencedObject.h"

/* Havok skinned mesh shape (8 bytes, DB types_members: hkReferencedObject base
   only — abstract interface; only passed through to the skinning util). */
struct hkMeshShape : hkReferencedObject
{
};
typedef struct hkMeshShape hkMeshShape;

/* DB types_enum_values, enum hkMeshShape::AccessFlags */
enum hkMeshShape_AccessFlags
{
    ACCESS_INDICES       = 1,
    ACCESS_VERTEX_BUFFER = 2,
};
