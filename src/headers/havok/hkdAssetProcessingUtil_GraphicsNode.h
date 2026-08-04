#pragma once
#include "hkRefPtr.h"

typedef struct hkdGraphicsShape hkdGraphicsShape;

/* size 8. hkdAssetProcessingUtil::GraphicsNode — a named graphics shape.
   Layout from DB type hkdAssetProcessingUtil::GraphicsNode. */
typedef struct hkdAssetProcessingUtil_GraphicsNode
{
    const char *m_name;                   /* 0 */
    hkRefPtr<hkdGraphicsShape> m_graphics;/* 4 */
} hkdAssetProcessingUtil_GraphicsNode;
