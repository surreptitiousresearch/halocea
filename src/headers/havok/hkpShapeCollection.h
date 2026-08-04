#pragma once

/* hkpShapeCollection and the hkpShape base it derives from, reproduced from the
   DB layout in plain-C form (self-contained, like hkpCollidable.h). Child shapes
   are only referenced through pointers, so hkpShape stays forward-declared for
   callers; the concrete base fields needed by hkpListShape accessors are laid out
   in hkpShapeBase. Layout verified against DB types_members. */

typedef struct hkpShape hkpShape;                 /* opaque — child shape pointers */
typedef struct hkpShapeContainer_vtbl hkpShapeContainer_vtbl;

/* hkpShapeCollection::CollectionType — DB types_enum_values. */
enum hkpShapeCollectionCollectionType
{
    COLLECTION_LIST                   = 0,
    COLLECTION_EXTENDED_MESH          = 1,
    COLLECTION_TRISAMPLED_HEIGHTFIELD = 2,
    COLLECTION_USER                   = 3,
    COLLECTION_MAX                    = 4,
};

/* hkpShape base concrete layout (size 16): hkReferencedObject (vtable +
   alloc/refcount) then m_userData and the hkEnum<hkpShapeType> storage word.
   hkpListShape overloads m_userData (child-array cache) and m_type (child count). */
typedef struct hkpShapeBase
{
    void *__vftable;                   /* 0  */
    unsigned short m_memSizeAndFlags;  /* 4  */
    short m_referenceCount;            /* 6  */
    unsigned int m_userData;           /* 8  */
    unsigned int m_type;               /* 12 hkEnum<hkpShapeType,unsigned int> storage */
} hkpShapeBase;

/* hkpShapeCollection (size 24). Base hkpShape then the multiply-inherited
   hkpShapeContainer vtable pointer, a welding flag, and the collection-type byte.
   Offset 22/23 are padding the list shape overloads for its disabled count. */
typedef struct hkpShapeCollection
{
    hkpShapeBase base;                        /* 0  */
    void *m_container_vftable;                /* 16 hkpShapeContainer::__vftable */
    unsigned char m_disableWelding;           /* 20 */
    unsigned char m_collectionType;           /* 21 */
    unsigned char _pad22[2];                  /* 22 */
} hkpShapeCollection;
