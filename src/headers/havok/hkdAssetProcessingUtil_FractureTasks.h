#pragma once

typedef struct hkdBreakableShape hkdBreakableShape;
typedef struct hkdFracture hkdFracture;

/* size 12. hkdAssetProcessingUtil::FractureTasks — one queued static-fracture
   task: which breakable shape to fracture, with which fracture, under what
   graphics-node name. Layout from DB type hkdAssetProcessingUtil::FractureTasks. */
typedef struct hkdAssetProcessingUtil_FractureTasks
{
    const char *m_name;         /* 0 */
    hkdBreakableShape *m_shape; /* 4 */
    hkdFracture *m_fracture;    /* 8 */
} hkdAssetProcessingUtil_FractureTasks;
