// ds::ByteReorder @0x82521C70 — in-place byte-reverse each `reorderGain`-sized element of `data`
// (an endian swap over `size / reorderGain` fixed-width elements). A no-op when reorderGain <= 1.
//
// The binary has two equivalent implementations selected by alignment: when `data` is 16-byte
// aligned AND `size` is a multiple of 32, a VMX (vperm128) fast path reverses 32 bytes per
// iteration; otherwise the portable scalar loop below runs. Only the scalar path is reproduced here
// — the VMX path computes the identical result and is treated as an optimization detail (boundary).

namespace ds {

void ByteReorder(unsigned char *data, int size, int reorderGain)
{
    if (reorderGain <= 1)
        return;

    // (aligned/32-multiple buffers take the VMX fast path in the binary; same result)
    int elementCount = size / reorderGain;
    for (int element = 0; element < elementCount; ++element) {
        unsigned char *tail = data + reorderGain;
        for (int i = 0; i < reorderGain / 2; ++i) {
            unsigned char tmp = data[i];
            data[i] = *(tail - 1);
            *--tail = tmp;
        }
        data += reorderGain;
    }
}

} // namespace ds
