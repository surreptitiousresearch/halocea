/* Re-evaluate the broadphase collision-filter relationship between a phantom
   (a1) and a collidable (a2) inside a container whose filter lives at a3+8.

   This is entirely virtual dispatch through opaque Havok objects that the
   decompiler could not type, so the raw vtable-offset arithmetic is preserved.
   Semantics recovered from the offsets:
     - a2 is an hkpCollidable: type tag  = *(a2+24) (m_broadPhaseHandle.m_type,
       2 => the collidable is itself a phantom), and its owner phantom is
       *(signed char*)(a2+16) + a2 (m_ownerOffset, the hkGetPhantom idiom).
     - phantom vtable: +28 addOverlappingCollidable, +32 getEnabled (writes a
       bool through the first arg), +36 removeOverlappingCollidable.
     - container filter (a3+8) vtable +4 = isCollisionEnabled (writes a bool).
   When the two collidables are mutually phantom, the mirrored add/remove is
   applied to the other phantom as well.

   The opaque objects have no reconstructed struct in the corpus; the raw vtable/
   member reads are kept as byte-exact cast-wrapped index expressions
   (((int*)p)[0], ((char*)c)[24], etc.) rather than invented member names. */
char *hkWorld_updateFilterOnSinglePhantom(int phantom, int collidable, int container)
{
    char enabledBefore;       /* v7  */
    char otherEnabledAdd;     /* v8  */
    char otherEnabledRemove;  /* v9  */
    char nowEnabled;          /* v10 */
    char *result;

    (*(void (__fastcall **)(char *, int, int))(((int *)phantom)[0] + 32))(&enabledBefore, phantom, collidable);
    result = (char *)(*(int (__fastcall **)(char *, int, int, int))(((int *)container)[2] + 4))(
                 &nowEnabled, container + 8, phantom + 16, collidable);

    if (*result)
    {
        if (!enabledBefore)
            result = (char *)(*(int (__fastcall **)(int, int))(((int *)phantom)[0] + 28))(phantom, collidable);

        if (((char *)collidable)[24] == 2)
        {
            int other = ((char *)collidable)[16] + collidable;
            result = (char *)(*(int (__fastcall **)(char *, int, int))(((int *)other)[0] + 32))(
                         &otherEnabledAdd, other, phantom + 16);
            if (!otherEnabledAdd)
                return (char *)(*(int (__fastcall **)(int, int))(((int *)other)[0] + 28))(other, phantom + 16);
        }
    }
    else
    {
        if (enabledBefore)
            result = (char *)(*(int (__fastcall **)(int, int))(((int *)phantom)[0] + 36))(phantom, collidable);

        if (((char *)collidable)[24] == 2)
        {
            int other = ((char *)collidable)[16] + collidable;
            result = (char *)(*(int (__fastcall **)(char *, int, int))(((int *)other)[0] + 32))(
                         &otherEnabledRemove, other, phantom + 16);
            if (otherEnabledRemove)
                return (char *)(*(int (__fastcall **)(int, int))(((int *)other)[0] + 36))(other, phantom + 16);
        }
    }
    return result;
}
