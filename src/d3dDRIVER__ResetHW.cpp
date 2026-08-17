/* ?ResetHW@d3dDRIVER@@MAAHXZ @0x82691010 */
#include "headers/d3d_driver.h"

// 0x82691010 -- d3dDRIVER::ResetHW. A thin forward to the Xenon device (re)creation path.
int d3dDRIVER::ResetHW() // decompiler typed BOOL; int per class decl
{
    return this->InitHWxenon();
}
