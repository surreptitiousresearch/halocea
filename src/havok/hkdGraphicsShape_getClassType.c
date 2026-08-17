/* ?getClassType@hkdGraphicsShape@@UBAPBVhkClass@@XZ @0x82EC0378 */
/* hkdGraphicsShape::getClassType — return the type's reflection descriptor.
   hkdGraphicsShape and hkClass are only referenced by pointer here, so they stay
   opaque (the existing hkClass.h / hkdGraphicsShape.h carry C++-only syntax that
   this C syntax check cannot parse). */
typedef struct hkdGraphicsShape hkdGraphicsShape;
typedef struct hkClass hkClass;

/* boundary — the reflection descriptor singleton for hkdGraphicsShape. */
extern const hkClass hkdGraphicsShapeClass; /* ?hkdGraphicsShapeClass@@3VhkClass@@B */

const hkClass *hkdGraphicsShape_getClassType(hkdGraphicsShape *this)
{
    (void)this;
    return &hkdGraphicsShapeClass;
}
