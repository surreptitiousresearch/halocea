#pragma once
#include "hkReferencedObject.h"
#include "hkEnum.h"

/* types_enum_values hkdController::ControllerType */
enum ControllerType
{
    CONTROLLER_TYPE_INVALID = 0,
    CONTROLLER_TYPE_CONTACTREGION = 1,
    CONTROLLER_TYPE_DEFORMATION = 2,
    CONTROLLER_TYPE_SPLITINHALF = 3,
    CONTROLLER_TYPE_WOOD = 4,
    CONTROLLER_TYPE_FLEXIBLE_JOINT = 5,
    CONTROLLER_TYPE_USER = 6,
    CONTROLLER_TYPE_NUM_TYPES = 7,
};

/* hkdController base (size 16). Verified against DB types_members. */
struct hkdController : hkReferencedObject
{
    hkEnum<ControllerType, unsigned char> m_type; /* 8  */
    unsigned char _pad09[3];                      /* 9  (alignment) */
    int m_unusedPadding;                          /* 12 */
};
typedef struct hkdController hkdController;
