#pragma once
/* hkVertexFormat — element-descriptor array (256B) + count @256 (DB size 260;
   stack footprints round it to 272). Layout from types_members hkVertexFormat /
   hkVertexFormat::Element. */

#include <stdint.h>
#include "hkEnum.h"
#include "hkFlags.h"

/* types_enum_values hkVertexFormat::DataType (DB enum, 4-byte; stored as u8) */
enum DataType
{
    TYPE_DESTR = 0,
    TYPE_DESTR_EVENT = 1,
    TYPE_DELETE = 2,
    TYPE_ACTION = 3,
    TYPE_LAST = 4,
};

/* types_enum_values hkVertexFormat::DataUsage (DB enum, 4-byte; stored as u8) */
enum DataUsage
{
    USAGE_NONE = 0,
    USAGE_POSITION = 1,
    USAGE_NORMAL = 2,
    USAGE_COLOR = 3,
    USAGE_TANGENT = 4,
    USAGE_BINORMAL = 5,
    USAGE_BLEND_MATRIX_INDEX = 6,
    USAGE_BLEND_WEIGHTS = 7,
    USAGE_BLEND_WEIGHTS_LAST_IMPLIED = 8,
    USAGE_TEX_COORD = 9,
    USAGE_POINT_SIZE = 10,
    USAGE_USER = 11,
    USAGE_LAST = 12,
};

/* types_enum_values hkVertexFormat::HintFlags (DB enum, 4-byte; stored as u8) */
enum HintFlags
{
    FLAG_READ = 1,
    FLAG_WRITE = 2,
    FLAG_DYNAMIC = 4,
    FLAG_NOT_SHARED = 8,
};

/* types_members hkVertexFormat::Element (8 bytes) */
typedef struct hkVertexFormat_Element
{
    hkEnum<DataType, unsigned char> m_dataType;   /* 0x00 */
    uint8_t m_numValues;                  /* 0x01 */
    hkEnum<DataUsage, unsigned char> m_usage;     /* 0x02 */
    uint8_t m_subUsage;                   /* 0x03 */
    hkFlags<HintFlags, unsigned char> m_flags;    /* 0x04 */
    uint8_t m_pad[3];                     /* 0x05 */
} hkVertexFormat_Element;

/* Local alias so member spellings match the DB's nested-qualified type
   (hkVertexFormat::Element). */
typedef hkVertexFormat_Element Element;

typedef struct hkVertexFormat
{
    Element m_elements[32]; /* 0x000 */
    int m_numElements;      /* 0x100 */
} hkVertexFormat;           /* 260 bytes */
