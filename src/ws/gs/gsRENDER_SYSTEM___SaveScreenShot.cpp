#include "../../headers/ws/gs/gsRENDER_SYSTEM.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM_boundary.h"
#include <time.h>

// 0x827A89AC — capture the current backbuffer to a BMP screenshot, honoring FORCE_SHOT / the
// "screenshot requested" state bit (0x08; a separate "sticky" bit 0x80 suppresses auto-clearing
// it). Two naming modes: in "anitec" (automated-test) mode — when gsAppSystem->anitecNames is
// non-empty — the path is built from screenShotPath + the current anitec map/test name + a
// zero-padded sequence number; otherwise it's screenShotPath + a timestamp. Every captured pixel
// is gamma-corrected via the D3D driver's PWL curve and its R/B channels swapped (BGR->RGB) before
// saving. `pCam` is unused (every call site in the corpus passes nullptr).
// DEVIATION: the decompiled body's dsTSTRING<char> ref-count increment/decrement/dlFree pairs
// around each concatenation are compiler-generated temporary-lifetime management for the
// (un-reversed) `operator+<char>` boundary helper, not meaningful custom logic — reconstructed
// here with plain dsTSTRING::operator+= concatenation, which is behaviorally equivalent.
void gsRENDER_SYSTEM::_SaveScreenShot(camCAMERA *pCam)
{
    (void)pCam;

    if (!FORCE_SHOT && ((state.state >> 3) & 1) == 0)
        return;

    unsigned int stateWord = state.state;
    FORCE_SHOT = false;
    if (((stateWord >> 7) & 1) == 0)
        state.state = stateWord & 0xFFFFFFF7u; // clear the "requested" bit unless sticky

    pctHEADER hdr = {};
    hdr.sx = 0;
    hdr.sy = 0;
    hdr.sign = 0;
    hdr.sz = 1;
    hdr.nFaces = 1;
    hdr.format = 0;
    hdr.nMipMap = 1;
    hdr.sx = vidDriver->pMode->sx;
    hdr.sy = vidDriver->pMode->sy;

    pctPICTURE *pic = pctCreate(&hdr);
    if (!pic)
        return;

    vidDriver->QueueScreenshot(pic);
    vidDriver->FlushScreenshotQueue(nullptr);
    vidDriver->QueueScreenshot(nullptr);

    dsTSTRING<char> path;
    path.UnsafeInitEmpty();

    if (gsAppSystem->anitecNames.nElem)
    {
        // Copy the current anitec test name, replacing '|' with '_' (matches the decompiled
        // Replace("|","_") call) via a manual pass rather than the un-reversed Replace() method.
        const dsTSTRING<char> &rawName = gsAppSystem->anitecNames.Back();
        dsTSTRING<char> testName;
        testName.UnsafeInit(rawName.pBuffer->str, -1, 0);
        char *nameBuf = testName.Lock(testName.pBuffer->strLen);
        for (int i = 0; i < testName.pBuffer->strLen; ++i)
            if (nameBuf[i] == '|')
                nameBuf[i] = '_';
        testName.Unlock();

        path.UnsafeInit(screenShotPath.pBuffer->str, -1, 0);
        path.Insert(path.pBuffer->strLen, "\\", -1);
        path.Insert(path.pBuffer->strLen, gsLevelName);
        path.Insert(path.pBuffer->strLen, "\\", -1);
        path.Insert(path.pBuffer->strLen, testName);

        if (!osFileIsExist(path.pBuffer->str))
            osFileMakeDir(path.pBuffer->str);

        int shotIdx = gsAppSystem->anitecShotIdx++;
        if (shotIdx)
        {
            dsTSTRING<char> fileName;
            dsSPrintf(&fileName, "%s\\sh%06i.bmp", path.pBuffer->str, shotIdx - 1);
            path = fileName;
        }
    }
    else
    {
        long long now;
        _time64(&now);
        tm *localTm = localtime64(&now);

        if (!osFileIsExist(screenShotPath.pBuffer->str))
            osFileMakeDir(screenShotPath.pBuffer->str);

        dsTSTRING<char> fileName;
        dsSPrintf(&fileName, "%s\\%02i_%02i (%02i-%02i-%02i).bmp",
                  screenShotPath.pBuffer->str,
                  localTm->tm_mon + 1, localTm->tm_mday,
                  localTm->tm_hour, localTm->tm_min, localTm->tm_sec);
        path = fileName;
    }

    // Build the [0,255] gamma-correction lookup table from the D3D driver's PWL gamma curve.
    unsigned char gammaLUT[256];
    for (int i = 0; i < 256; ++i)
    {
        float linear = (float)i * 0.0039215689f; // i / 255
        float corrected = d3dDriver->ConvertPWL2ProperGamma(linear);
        gammaLUT[i] = (unsigned char)(corrected * 255.0f);
    }

    for (int y = hdr.sy - 1; y >= 0; --y)
    {
        for (int x = 0; x < hdr.sx; ++x)
        {
            pctPIXEL px;
            pctGetPixel(pic, x, y, 0, &px, 0, 0);
            unsigned char newG = gammaLUT[px.___u2.g];
            unsigned char newB = gammaLUT[px.___u3.b];
            px.___u1.r = gammaLUT[px.___u1.r];
            px.___u2.g = newG;
            px.___u3.b = newB;
            pctSetPixel(pic, x, y, 0, &px, 0, 0);
        }
    }

    if (pctSaveBMP(path.pBuffer->str, pic))
        ++screenShotNmb;

    pctDestroy(pic);
}
