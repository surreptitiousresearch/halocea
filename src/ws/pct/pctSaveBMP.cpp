#include "../../headers/pctPICTURE.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM_boundary.h" /* pctPIXEL, pctGetPixel */
#include "../../headers/ws/fio/fioFILE_DISK.h"
#include "../../headers/ws/fio/FIO_OPEN.h"

// Little-endian field writers. The target is big-endian PowerPC while the BMP
// on-disk format is little-endian, so every multi-byte header field is emitted
// byte-by-byte. The shipped decompiler renders this as scattered BYTEn()/HIBYTE()
// packs into stack ints; reconstructed here as explicit LE stores, which is
// behaviorally identical.
static void pctPutLE16(unsigned char *p, unsigned int value)
{
    p[0] = (unsigned char)value;
    p[1] = (unsigned char)(value >> 8);
}

static void pctPutLE32(unsigned char *p, unsigned int value)
{
    p[0] = (unsigned char)value;
    p[1] = (unsigned char)(value >> 8);
    p[2] = (unsigned char)(value >> 16);
    p[3] = (unsigned char)(value >> 24);
}

// 0x82D4CF20 — write a picture to a 24-bpp uncompressed Windows BMP at `fname`.
// Rows are stored bottom-up (BMP convention) and each pixel is fetched via
// pctGetPixel and stored B,G,R. Returns 1 on success, 0 if the file could not be
// opened. Source: D:\Projects\code\common\src.sys\picture\pic_bmp.cpp
int pctSaveBMP(const char *fname, pctPICTURE *pPict)
{
    fioFILE_DISK file;

    if (!file.InitFile(fname, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_W}, -1))
        return 0;

    int sx = pPict->hdr.sx;
    int sy = pPict->hdr.sy;

    // 54-byte header (14 + 40) + 3 bytes per pixel; 18 == 54/3 folded into the
    // per-pixel term by the original.
    unsigned int fileSize = 3 * (sy * sx + 18);

    unsigned char *rgb = new unsigned char[3 * sy * sx]; // pic_bmp.cpp:213

    // BITMAPFILEHEADER (14 bytes)
    unsigned char fileHeader[14];
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    pctPutLE32(fileHeader + 2, fileSize);   // bfSize
    pctPutLE16(fileHeader + 6, 0);          // bfReserved1
    pctPutLE16(fileHeader + 8, 0);          // bfReserved2
    pctPutLE32(fileHeader + 10, 54);        // bfOffBits (pixel data offset)

    // BITMAPINFOHEADER (40 bytes)
    unsigned char infoHeader[40];
    pctPutLE32(infoHeader + 0, 40);         // biSize
    pctPutLE32(infoHeader + 4, sx);         // biWidth
    pctPutLE32(infoHeader + 8, sy);         // biHeight
    pctPutLE16(infoHeader + 12, 1);         // biPlanes
    pctPutLE16(infoHeader + 14, 24);        // biBitCount
    pctPutLE32(infoHeader + 16, 0);         // biCompression (BI_RGB)
    pctPutLE32(infoHeader + 20, 0);         // biSizeImage
    pctPutLE32(infoHeader + 24, 0x2B10);    // biXPelsPerMeter (~72 dpi)
    pctPutLE32(infoHeader + 28, 0x2B10);    // biYPelsPerMeter
    pctPutLE32(infoHeader + 32, 0);         // biClrUsed
    pctPutLE32(infoHeader + 36, 0);         // biClrImportant

    file.WriteData(fileHeader, 14, 1);
    file.WriteData(infoHeader, 40, 1);

    int cursor = 0;
    for (int y = sy - 1; y >= 0; --y)
    {
        for (int x = 0; x < sx; ++x, cursor += 3)
        {
            pctPIXEL pixel;
            pctGetPixel(pPict, x, y, 0, &pixel, 0, 0);
            rgb[cursor]     = pixel.___u3.b;
            rgb[cursor + 1] = pixel.___u2.g;
            rgb[cursor + 2] = pixel.___u1.r;
        }
    }

    file.WriteData(rgb, cursor, 1);
    file.Term();
    delete[] rgb;
    return 1;
}
