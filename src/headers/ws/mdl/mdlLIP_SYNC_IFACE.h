#pragma once
// ws-engine mdl: abstract lip-sync interface. DB-verified layout (types_members mdlLIP_SYNC_IFACE /
// mdlLIP_SYNC_IFACE_vtbl) — size 4 (single vptr). Two virtual slots: dtr and ApplyLipSync.

struct mdlLIP_SYNC_IFACE;

struct mdlLIP_SYNC_IFACE_vtbl {
    void (__fastcall *dtr_mdlLIP_SYNC_IFACE)(mdlLIP_SYNC_IFACE *self); // slot 0
    void (__fastcall *ApplyLipSync)(mdlLIP_SYNC_IFACE *self);         // slot 1
};

struct mdlLIP_SYNC_IFACE {
    mdlLIP_SYNC_IFACE_vtbl *__vftable; // 0x00

    virtual ~mdlLIP_SYNC_IFACE();
    virtual void ApplyLipSync() = 0;
};
