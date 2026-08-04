#pragma once
#include "../ws/ui_new/UI_FRAME_DESC.h"
// HCEX bridge: the dscDESC (brand descriptor) for HCEX_LOADING_SCREEN, registered with
// ui_new::uiFamily under the "hcex_loading_screen" brand name. Adds no fields of its own over
// ui_new::UI_FRAME_DESC. DB-verified layout (types_members HCEX_LOADING_SCREEN_DESC):
// ui_new::UI_FRAME_DESC (base)@0 — size 24.
// DB-verified vtable layout (types_members HCEX_LOADING_SCREEN_DESC_vtbl): GetCurTypeInfo@0,
// dtr@4, NotifyTermBrand@8, ParsePS@12, PostProcessPS@16, GetSslClass@20, InitSslClass@24,
// UnshareSslClass@28, IsNeedToUnshareSslClass@32.

struct dscTYPE_INFO; // boundary — dsc subsystem
struct psSECTION;    // boundary — ps (parse-section) subsystem
struct sslCLASS_REF;
struct HCEX_LOADING_SCREEN_DESC;

typedef struct HCEX_LOADING_SCREEN_DESC_vtbl {
    const dscTYPE_INFO *(__fastcall *GetCurTypeInfo)(HCEX_LOADING_SCREEN_DESC *self);            // 0x00
    void                (__fastcall *dtr_HCEX_LOADING_SCREEN_DESC)(HCEX_LOADING_SCREEN_DESC *self); // 0x04
    void                (__fastcall *NotifyTermBrand)(HCEX_LOADING_SCREEN_DESC *self);            // 0x08
    void                (__fastcall *ParsePS)(HCEX_LOADING_SCREEN_DESC *self, psSECTION *section, const dsTSTRING<char> *hintErr);       // 0x0C
    void                (__fastcall *PostProcessPS)(HCEX_LOADING_SCREEN_DESC *self, psSECTION *section, const dsTSTRING<char> *hintErr); // 0x10
    sslCLASS_REF       *(__fastcall *GetSslClass)(HCEX_LOADING_SCREEN_DESC *self, sslCLASS_REF *result); // 0x14
    int                 (__fastcall *InitSslClass)(HCEX_LOADING_SCREEN_DESC *self, const dsTSTRING<char> *sslDecl);                     // 0x18
    void                (__fastcall *UnshareSslClass)(HCEX_LOADING_SCREEN_DESC *self, bool a, bool b); // 0x1C
    bool                (__fastcall *IsNeedToUnshareSslClass)(HCEX_LOADING_SCREEN_DESC *self, bool a); // 0x20
} HCEX_LOADING_SCREEN_DESC_vtbl;

struct HCEX_LOADING_SCREEN_DESC : ui_new::UI_FRAME_DESC {
    static HCEX_LOADING_SCREEN_DESC_vtbl vftable; // `HCEX_LOADING_SCREEN_DESC::`vftable'' — boundary

    // 0x823F40D8 — chain to dscDESC's ctor (which transiently sets __vftable to sslDESC's, per
    // the sslDESC/dscDESC construction-order idiom), zero isUnshared, construct sslClass, zero
    // isSslClassInited, then set this class's own vtable.
    HCEX_LOADING_SCREEN_DESC();
    // 0x823F4140 — chain to ~sslCLASS_REF then ~dscDESC.
    ~HCEX_LOADING_SCREEN_DESC();
    // 0x823F4178 — scalar deleting destructor: run the dtor chain above, then `operator delete`
    // this object if the low bit of `freeMemory` is set.
    HCEX_LOADING_SCREEN_DESC *ScalarDeletingDestructor(unsigned int freeMemory);
    // 0x823F4550 — member-wise copy: pBrand, isUnshared, sslClass (via its own operator=),
    // isSslClassInited.
    HCEX_LOADING_SCREEN_DESC &operator=(const HCEX_LOADING_SCREEN_DESC &that);
};
