#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
/* hkClassMember::Type — reflection type-code, stored as a 1-byte hkEnum but
   promoted to a 4-byte slot in the properties table. The value labels below are
   reproduced verbatim from the database's enum annotation. */
typedef enum hkClassMemberType
{
    TYPE_INT                     = 0,
    TYPE_BOOL                    = 1,
    TYPE_FLOAT                   = 2,
    TYPE_DSSTRID                 = 3,
    TYPE_DS_DATA                 = 4,
    TYPE_SSLOBJECT_REF           = 5,
    TYPE_SSLOBJECT_SUBELEM_REF   = 6,
    TYPE_SSLFUNC_PARAM_REF_STACK = 7,
    TYPE_SSLFUNC_PARAM_REF_ARR   = 8,
    TYPE_SSLFUNC_CONST_REF       = 9,
    TYPE_SSLFUNC_LOCAL_VAR_REF   = 10,
    TYPE_DSDATA_MEMBER_REF       = 11
} hkClassMemberType;

/* hkClassMember::TypeProperties — one row of the ClassMemberProperties table:
   the printable name plus on-disk size/alignment for each member type. (12 bytes) */
typedef struct hkClassMemberTypeProperties
{
    unsigned char m_type;   /* hkEnum<hkClassMemberType, unsigned char>; occupies a 4-byte slot */
    char _pad[3];
    const char *m_name;
    short m_size;
    short m_align;
} hkClassMemberTypeProperties;
