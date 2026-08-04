#pragma once
typedef struct hkxAttributeGroup hkxAttributeGroup;
/* size 8 */
typedef struct hkxAttributeHolder
{
    hkxAttributeGroup *m_attributeGroups;
    int m_numAttributeGroups;
} hkxAttributeHolder;
