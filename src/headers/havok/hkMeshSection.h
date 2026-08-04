#pragma once
#include "hkEnum.h"

typedef struct hkMeshVertexBuffer hkMeshVertexBuffer;
typedef struct hkMeshMaterial hkMeshMaterial;

/* types_enum_values hkMeshSection::PrimitiveType (DB enum, 4-byte; stored as u8 via hkEnum) */
enum PrimitiveType
{
    PRIMITIVE_TYPE_UNKNOWN = 0,
    PRIMITIVE_TYPE_POINT_LIST = 1,
    PRIMITIVE_TYPE_LINE_LIST = 2,
    PRIMITIVE_TYPE_TRIANGLE_LIST = 3,
    PRIMITIVE_TYPE_TRIANGLE_STRIP = 4,
};

/* types_enum_values hkMeshSection::MeshSectionIndexType (DB enum, 4-byte; stored as u8 via hkEnum) */
enum MeshSectionIndexType
{
    INDEX_TYPE_NONE = 0,
    INDEX_TYPE_UINT16 = 1,
    INDEX_TYPE_UINT32 = 2,
};

/* size 40 */
typedef struct hkMeshSection
{
    hkEnum<PrimitiveType, unsigned char> m_primitiveType; /* 0  */
    unsigned char _pad01[3];            /* 1  */
    int m_numPrimitives;                /* 4  */
    int m_numIndices;                   /* 8  */
    int m_vertexStartIndex;             /* 12 */
    int m_transformIndex;               /* 16 */
    hkEnum<MeshSectionIndexType, unsigned char> m_indexType; /* 20 */
    unsigned char _pad15[3];            /* 21 */
    const void *m_indices;              /* 24 */
    hkMeshVertexBuffer *m_vertexBuffer; /* 28 */
    hkMeshMaterial *m_material;         /* 32 */
    int m_sectionIndex;                 /* 36 */
} hkMeshSection;
