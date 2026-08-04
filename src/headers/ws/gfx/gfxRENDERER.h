#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../m3d/m3dCOLOR.h"
#include "../m3d/m3dVTX.h"
#include "gfxCounters.h"
// ws-engine gfx: the Scaleform GFx renderer backend (derives the third-party GRenderer base).
// Full DB-verified member sequence (types_members gfx::gfxRENDERER) — size 700 (0x2BC).
// The third-party Scaleform SDK member types (GRenderer, GMatrix2D, GRenderer::Cxform, GArray,
// GRenderer::* enums, GTexture, GRenderer::BitmapDesc) are boundary types spelled verbatim per
// the DB and forward-declared here — their own bodies are SDK-owned boundaries.

struct txmTEXTURE; // txm subsystem texture (pointer only)

// --- third-party Scaleform GFx SDK boundary types ---
// Layouts reconciled against the DB 2026-08-04 (types_members for each type, cited inline).
// The types remain SDK-owned boundaries; only their DB-recorded layouts are modeled here.
struct GMatrix2D { float M_[2][3]; };  // DB: M_@0 float[2][3] — size 24 (2x3 affine matrix)
struct GTexture;                       // DB size 8 — used only by pointer, fwd-decl suffices
struct GRendererEventHandler;          // SDK boundary — used only by pointer (GList node links)

// DB types_members GRect<float>: Left/Top/Right/Bottom — size 16.
template<class T> struct GRect {
    T Left;   // 0x00
    T Top;    // 0x04
    T Right;  // 0x08
    T Bottom; // 0x0C
};
// GColor — DB size 4; the single member is an anonymous union with no named interior
// (types_members checked 2026-08-04), so the 4B body stays opaque. SDK boundary.
struct GColor { unsigned char _opaque[4]; };

// GFx refcount base chain (DB: GRefCountBase<GRenderer,65> -> GRefCountBaseStatImpl
// <GRefCountImpl,65> -> GRefCountImpl -> GRefCountImplCore {__vftable@0, RefCount@4} — 8B).
struct GRefCountImplCore_vtbl; // SDK boundary vtable
struct GRefCountImplCore {
    GRefCountImplCore_vtbl *__vftable; // 0x00
    volatile int            RefCount;  // 0x04
};
struct GRefCountImpl : GRefCountImplCore {};
template<class Base, int Stat> struct GRefCountBaseStatImpl : Base {};
template<class T, int Stat> struct GRefCountBase : GRefCountBaseStatImpl<GRefCountImpl, Stat> {};

// GFx intrusive list (DB: GList<GRendererEventHandler> {Root@0 GListNode (8B)}).
template<class T> struct GListNode { T *pPrev; T *pNext; };
template<class T> struct GList { GListNode<T> Root; };

// GFx renderer base; DB size 40. Nested value types (Cxform/BitmapDesc/StereoParams) and enums
// are declared inside so the qualified names GRenderer::* used by gfxRENDERER resolve.
struct GRenderer : GRefCountBase<GRenderer, 65> {          // base 0x00 (8B)
    enum GouraudFillType : int;   // DB size 4
    enum BitmapSampleMode : int;  // DB size 4
    enum BlendType : int;         // DB size 4
    enum VertexFormat : int;      // DB size 4
    enum IndexFormat : int;       // DB size 4
    // DB types_enum_values GRenderer::StereoDisplay.
    enum StereoDisplay : int { StereoCenter = 0, StereoLeft = 1, StereoRight = 2 };
    struct Cxform { float M_[4][2]; };                 // DB: M_@0 float[4][2] — size 32
    // DB: Coords@0, TextureCoords@16 (GRect<float>), Color@32 (GColor) — size 36.
    struct BitmapDesc {
        GRect<float> Coords;        // 0x00
        GRect<float> TextureCoords; // 0x10
        GColor       Color;         // 0x20
    };
    // DB types_members GRenderer::StereoParams — 5 floats, size 20.
    struct StereoParams {
        float DisplayWidthCm;     // 0x00
        float Distortion;         // 0x04
        float DisplayDiagInches;  // 0x08
        float DisplayAspectRatio; // 0x0C
        float EyeSeparationCm;    // 0x10
    };

    GList<GRendererEventHandler> Handlers;   // 0x08
    StereoParams                 S3DParams;  // 0x10 (20B)
    StereoDisplay                S3DDisplay; // 0x24
}; // 40 bytes

// GFx array template chain (DB: GArray -> GArrayBase<GArrayData<T,GAllocatorGH<T,N>,Policy>> ->
// GArrayData -> GArrayDataBase {Data@0, Size@4, Policy@8} — 12B).
struct GArrayDefaultPolicy { unsigned int Capacity; }; // DB: Capacity@0 — size 4
template<class T, int N> struct GAllocatorGH {};       // SDK allocator tag (no DB members)
template<class T, class Allocator, class P> struct GArrayDataBase {
    T           *Data;   // 0x00
    unsigned int Size;   // 0x04
    P            Policy; // 0x08
};
template<class T, class Allocator, class P> struct GArrayData : GArrayDataBase<T, Allocator, P> {};
template<class T> struct GArrayBase { T Data; };
template<class T, int N, class Policy>
struct GArray : GArrayBase<GArrayData<T, GAllocatorGH<T, N>, Policy> > {}; // DB size 12

// GRenderer nested value/enum types used by value:
//   GRenderer::Cxform (32B), GRenderer::BitmapDesc (36B), and the enums
//   GouraudFillType / BitmapSampleMode / BlendType / VertexFormat / IndexFormat.
// GArray<...> is a GFx array template (12B); dsCONST_ARRAY / dsVECTOR are our own.

namespace gfx {

struct gfxTEXTURE; // boundary — gfx subsystem, only used by pointer here

typedef struct gfxRENDERER {
    // gfxRENDERER-local nested types (referenced as members below).
    // gfx::gfxRENDERER::UI_VTX — DB-verified: x@0, y@4, diff@8, tc@12 (m3dVTX) — size 20.
    struct UI_VTX {
        float        x;    // 0x00
        float        y;    // 0x04
        unsigned int diff; // 0x08 diffuse color
        m3dVTX       tc;   // 0x0C texture coordinate
    };
    enum FILL_STYLE : int;    // gfxRENDERER::FILL_STYLE — DB size 4
    enum DRAW_TYPE : int;     // gfxRENDERER::DRAW_TYPE — DB size 4

    GRenderer                base;                     // 0x000 (40B baseclass)
    dsVECTOR<gfxTEXTURE *, 8> m_TexturesToBeActualized; // 0x028
    gfxCounters              m_Counters;                // 0x03C
    GMatrix2D                m_CurrentMatrix;           // 0x060
    GMatrix2D                m_UserMatrix;              // 0x078
    float                    m_SplitScreenOffsetX;      // 0x090
    float                    m_SplitScreenOffsetY;      // 0x094
    GRenderer::Cxform        m_CurrentCxform;           // 0x098 (32B)
    float                    m_ScreenSizeX;             // 0x0B8
    float                    m_ScaleX;                  // 0x0BC
    float                    m_ScreenSizeY;             // 0x0C0
    float                    m_ScaleY;                  // 0x0C4
    FILL_STYLE               m_FillStyle;               // 0x0C8
    GRenderer::GouraudFillType m_GouraudSubstyle;       // 0x0CC
    m3dCOLOR                 m_FillColor;               // 0x0D0
    GTexture                *m_FillTexture;             // 0x0E0
    GRenderer::BitmapSampleMode m_TextureSampleMode;    // 0x0E4
    GMatrix2D                m_FillTextureMatrix;       // 0x0E8
    m3dCOLOR                 m_LineColor;               // 0x100
    bool                     m_SubmittingMask;          // 0x110
    bool                     m_DrawingWithMask;         // 0x111
    unsigned char            _pad112[2];                // 0x112 db-verified padding
    GRenderer::BlendType     m_BlendMode;               // 0x114
    GArray<enum GRenderer::BlendType,2,GArrayDefaultPolicy> m_BlendModeStack; // 0x118 (12B)
    const void              *m_pVertices;               // 0x124
    int                      m_numVertices;             // 0x128
    GRenderer::VertexFormat  m_VF;                      // 0x12C
    dsVECTOR<gfxRENDERER::UI_VTX, 8> g_VertexBuffer;    // 0x130
    const void              *m_pIndices;                // 0x144
    int                      m_numIndices;              // 0x148
    GRenderer::IndexFormat   m_IF;                      // 0x14C
    dsVECTOR<unsigned short, 8> g_IndexBuffer;          // 0x150
    int                      m_RenderCall;              // 0x164
    int                      m_RenderCallOfInterest;    // 0x168
    DRAW_TYPE                m_DrawType;                // 0x16C
    unsigned int             m_DrawSequence;            // 0x170
    dsVECTOR<unsigned char, 8> m_SavedVertexBuffer;     // 0x174
    unsigned int             m_SavedNVertices;          // 0x188
    GRenderer::VertexFormat  m_SavedVF;                 // 0x18C
    dsVECTOR<unsigned short, 8> m_SavedIndexBuffer;     // 0x190
    unsigned int             m_SavedNIndices;           // 0x1A4
    GRenderer::IndexFormat   m_SavedIF;                 // 0x1A8
    GMatrix2D                m_SavedCurrentMatrix;      // 0x1AC
    bool                     m_CurrentMatrixDirty;      // 0x1C4
    unsigned char            _pad1C5[3];                // 0x1C5 db-verified padding
    GMatrix2D                m_SavedUserMatrix;         // 0x1C8
    bool                     m_UserMatrixDirty;         // 0x1E0
    unsigned char            _pad1E1[3];                // 0x1E1 db-verified padding
    GRenderer::Cxform        m_SavedCxform;             // 0x1E4 (32B)
    bool                     m_CurrentCxformDirty;      // 0x204
    unsigned char            _pad205[3];                // 0x205 db-verified padding
    GRenderer::BlendType     m_SavedBlendMode;          // 0x208
    FILL_STYLE               m_SavedFillStyle;          // 0x20C
    GRenderer::GouraudFillType m_SavedGouraudSubstyle;  // 0x210
    m3dCOLOR                 m_SavedFillColor;          // 0x214
    bool                     m_FillColorDirty;          // 0x224
    unsigned char            _pad225[3];                // 0x225 db-verified padding
    GTexture                *m_SavedFillTexture0;       // 0x228
    GTexture                *m_SavedFillTexture1;       // 0x22C
    GRenderer::BitmapSampleMode m_SavedTextureSampleMode;// 0x230
    GMatrix2D                m_SavedFillTextureMatrix;  // 0x234
    bool                     m_FillTextureMatrixDirty;  // 0x24C
    unsigned char            _pad24D[3];                // 0x24D db-verified padding
    m3dCOLOR                 m_SavedLineColor;          // 0x250
    bool                     m_LineColorDirty;          // 0x260
    unsigned char            _pad261[3];                // 0x261 db-verified padding
    dsVECTOR<GRenderer::BitmapDesc, 8> m_SavedBitmapRectangles; // 0x264
    unsigned int             m_SavedNRectangles;        // 0x278
    const GTexture          *m_SavedBitmapTexture;      // 0x27C
    GMatrix2D                m_SavedBitmapMatrix;       // 0x280
    bool                     m_BitmapMatrixDirty;       // 0x298
    unsigned char            _pad299[3];                // 0x299 db-verified padding
    float                    m_StereoDepth;             // 0x29C
    float                    m_SavedStereoDepth;        // 0x2A0
    dsCONST_ARRAY<txmTEXTURE *, 5> m_pVideoTextures;    // 0x2A4 (24B)

    // 0x8332DA74 — reversed in gfxRENDERER__GetCounters.cpp.
    void GetCounters(gfxCounters &rCounters);
    // 0x8332DA54 — reversed in gfxRENDERER__ResetFrameCounters.cpp.
    void ResetFrameCounters();
} gfxRENDERER;

} // namespace gfx
