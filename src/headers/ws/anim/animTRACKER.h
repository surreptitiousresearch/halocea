#pragma once
#include "../m3d/m3dMATR.h"
// ws-engine anim: follow tracker — drives an animINST to track another object/camera/matrix.
// DB-verified layout (types_members animTRACKER) — size 88 (0x58).

struct animINST; // animINST.h — tracked / owning instance (pointer only)  boundary
struct objOBJ;   // obj subsystem model object (pointer only)               boundary
struct camCAMERA;// cam subsystem camera (pointer only)                      boundary

typedef struct animTRACKER {
    // Tracking source discriminator (DB member type TRK_TYPE).
    // DB-verified via types_enum_values TRK_TYPE (int-sized enum, layout unchanged).
    enum TRK_TYPE {
        TRK_NA = 0,
        TRK_POS = 1,
        TRK_MATRIX = 2,
        TRK_INST = 3,
        TRK_INST_OBJ = 4,
        TRK_INST_MATRIX = 5,
        TRK_CAMERA = 6,
        TRK_INST_OBJ_MATRIX = 7,
        TRK_INST_POS = 8,
        TRK_INST_OBJ_POS = 9,
        TRK_LAST = 10,
    };

    animINST  *pInst;      // 0x00 instance being driven
    objOBJ    *pObj;       // 0x04 tracked object
    camCAMERA *pCamera;    // 0x08 tracked camera
    TRK_TYPE   type;       // 0x0C which source is active
    m3dMATR    matr;       // 0x10 tracking transform
    m3dMATR   *mOffset;    // 0x50 optional offset transform
    animINST  *pInstOwner; // 0x54 owning instance

    // 0x82B2F110 — resolve the world transform this tracker follows into `out`, dispatching on
    // `type` (matrix / instance / object / camera / origin-of-object variants). Strips scale and
    // applies the optional offset. Returns false for an unknown type. Const (QBA).
    bool GetMatrix(m3dMATR *out) const;

protected:
    // 0x82B2EB48 — protected const (IBA): fetch the world transform to follow for `pInst`.
    void GetInstMatrix(animINST *pInst, m3dMATR *out) const;
} animTRACKER;
