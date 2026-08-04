#pragma once
#include "hkArray.h"

/* hkpPropertyValue — an 8-byte tagged scalar/pointer union carried by a property
   (DB types_members hkpPropertyValue, single 8-byte m_data). */
typedef struct hkpPropertyValue
{
    unsigned __int64 m_data; /* 0 */
} hkpPropertyValue;

/* hkpProperty — a key/value pair in the property array (16 bytes, DB
   types_members hkpProperty). */
typedef struct hkpProperty
{
    unsigned int m_key;            /* 0 */
    unsigned int m_alignmentPadding; /* 4 */
    hkpPropertyValue m_value;      /* 8 */
} hkpProperty;

/* hkdProperties (size 12, DB types_members) — a growable set of hkpProperty. */
typedef struct hkdProperties
{
    hkArray<hkpProperty> m_properties; /* 0 */
} hkdProperties;

/* Boundary callees (2-level descent). */
extern void hkpProperty_mapStringToKey(const char *name, unsigned int *keyOut);       /* hkpProperty::mapStringToKey */
extern void hkdProperties_addProperty(hkdProperties *self, unsigned int key, hkpPropertyValue *value); /* hkdProperties::addProperty */
