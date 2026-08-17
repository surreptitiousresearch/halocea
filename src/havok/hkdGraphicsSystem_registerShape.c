/* ?registerShape@hkdGraphicsSystem_S3D@@UAAXPBVhkdGraphicsShape@@PBD@Z @0x82EC0450 */
/* hkdGraphicsSystem_S3D::registerShape — no-op in this build (the S3D graphics
   system does not track registered shapes; the body compiled away to nothing).
   The parameters are only referenced by pointer, so the types stay opaque. */
typedef struct hkdGraphicsSystem hkdGraphicsSystem;
typedef struct hkdGraphicsShape hkdGraphicsShape;

void hkdGraphicsSystem_registerShape(hkdGraphicsSystem *this, const hkdGraphicsShape *entity,
                                     const char *shapeName)
{
    (void)this;
    (void)entity;
    (void)shapeName;
}
