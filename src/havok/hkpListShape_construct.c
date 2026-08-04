#include "../headers/havok/hkpListShape.h"

/* Boundary callees / data (2-level descent limit). */
extern void hkpShapeCollection_construct(hkpShapeCollection *self, int shapeType, int collectionType); /* hkpShapeCollection::hkpShapeCollection */
extern void hkArrayUtil__reserve(void *arrayData, int numElem, int sizeElem);                   /* hkArrayUtil::_reserve — arg0 void* per mangled ?_reserve@hkArrayUtil@@YAXPAXHH@Z */
extern void hkpListShape_setShapes(hkpListShape *self, const hkpShape *const *shapeArray, int numShapes,
                                   const void *perShapeInfo, int referencePolicy);                     /* hkpListShape::setShapes */
extern void *hkpListShape_vftable_for_hkpShape[];          /* hkpListShape::`vftable'{for `hkpShape'} */
extern void *hkpListShape_vftable_for_hkpShapeContainer[]; /* hkpListShape::`vftable'{for `hkpShapeContainer'} */

#define HK_SHAPE_LIST 9 /* hkpShapeType HK_SHAPE_LIST */

/* hkpListShape::hkpListShape — construct a list shape over an array of child
   shapes. Runs the collection base ctor (tagged HK_SHAPE_LIST / COLLECTION_LIST),
   installs the two vtables, primes the child-info array (reserve 16 elems), then
   loads the children via setShapes. Finishes by marking every one of the eight
   enabled-children bitset words as all-ones (all children enabled) and clearing
   the disabled count and flags.
   DEVIATION: the bitset fill is a scalar loop over m_enabledChildren[0..7]; the
   original walks from &m_aabbCenter.quad.u32[3] via *++ptr. */
void hkpListShape_construct(hkpListShape *this, const hkpShape *const *shapeArray, int numShapes,
                           int referencePolicy)
{
    int i;

    hkpShapeCollection_construct(&this->base, HK_SHAPE_LIST, COLLECTION_LIST);
    this->base.base.__vftable = hkpListShape_vftable_for_hkpShape;
    this->base.m_container_vftable = hkpListShape_vftable_for_hkpShapeContainer;

    this->m_childInfo.m_data = 0;
    this->m_childInfo.m_size = 0;
    this->m_childInfo.m_capacityAndFlags = (int)0x80000000;
    hkArrayUtil__reserve(&this->m_childInfo.m_data, 4, 16);

    hkpListShape_setShapes(this, shapeArray, numShapes, 0, referencePolicy);

    for (i = 0; i < 8; ++i)
        this->m_enabledChildren[i] = 0xFFFFFFFFu;

    this->m_numDisabledChildren = 0;
    this->m_flags = 0;
}
