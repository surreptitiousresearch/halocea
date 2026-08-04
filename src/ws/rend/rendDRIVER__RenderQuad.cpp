#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/rend/rendV3D.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

// rendDRIVER::RenderQuad @ 0x827CA218
// Assemble a 4-vertex screen-space quad spanning corners (x1,y1)-(x2,y2) with texture
// coordinates (s1,t1)-(s2,t2), all at constant depth z, then submit it via RenderPoly.
//
// Note on the signature: the decompiler mis-attributed the ABI and printed ~44 parameters
// plus duplicated blendMode/color/texture as a40/a42/a44. The disasm (0x827CA260-0x827CA270)
// shows the RenderPoly call actually reads the incoming stack args texture(arg_74),
// color(arg_6C) and blendMode(arg_67, a byte); the nine floats are f1..f9
// (x1,y1,s1,t1,x2,y2,s2,t2,z). Corrected to that real signature.
//
// Vertex winding derived from the store pattern at 0x827CA228-0x827CA298: each vertex takes
// color 0xFFFFFFFF (per-vertex modulate handled by the separate `color` passed to RenderPoly).
void rendDRIVER::RenderQuad(float x1, float y1, float s1, float t1,
                            float x2, float y2, float s2, float t2, float z,
                            PASS_DESC_BASE::BLENDMODE blendMode, unsigned int color,
                            txmTEXTURE *texture)
{
    rendV3D quad[4];

    // vertex 0: top-left corner
    quad[0].v.x = x1; quad[0].v.y = y1; quad[0].v.z = z;
    quad[0].color = 0xFFFFFFFFu;
    quad[0].tc.s = s1; quad[0].tc.t = t1;

    // vertex 1: bottom-left corner
    quad[1].v.x = x1; quad[1].v.y = y2; quad[1].v.z = z;
    quad[1].color = 0xFFFFFFFFu;
    quad[1].tc.s = s1; quad[1].tc.t = t2;

    // vertex 2: bottom-right corner
    quad[2].v.x = x2; quad[2].v.y = y2; quad[2].v.z = z;
    quad[2].color = 0xFFFFFFFFu;
    quad[2].tc.s = s2; quad[2].tc.t = t2;

    // vertex 3: top-right corner
    quad[3].v.x = x2; quad[3].v.y = y1; quad[3].v.z = z;
    quad[3].color = 0xFFFFFFFFu;
    quad[3].tc.s = s2; quad[3].tc.t = t1;

    RenderPoly(4, quad, blendMode, color, texture);
}
