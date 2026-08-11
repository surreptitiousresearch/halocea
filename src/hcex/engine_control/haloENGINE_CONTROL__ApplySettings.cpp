// haloENGINE_CONTROL__ApplySettings @0x823C7F20
#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"
#include "../../headers/button_preset_flags.h"

// haloENGINE_CONTROL::ApplySettings @ 0x823C7F40 — apply one user slot's (primary or secondary)
// persisted controller settings from its per-user apCONFIG onto its live player_profile
// (invert-look/flight/autocenter, look-sensitivity, joystick-vibration-off, and the named
// button/stick layout presets).
//
// CAVEAT: the decompiler mistypes its `ds_data::REF_TYPE<apCONFIG>` local (4 bytes, returned by
// gsUSER_CFG_MNG::Get's hidden result pointer) as a whole `gsUSER_CFG_MNG` value (40 bytes) — a PPC
// struct-return-by-value ABI confusion (the real call is `gsUserCfgMng->Get(&userConfigRef, userIdx)`,
// which the decompiler shows with `this`/`result` swapped and the result over-typed). Every
// subsequent `v19.eventDispatcher.pObj` / `v19.defaultCfg.m_pData` / `v19.tasks.head` etc. read in
// the raw decompile is the *same* 4-byte handle re-read through different (wrong) field offsets of
// the over-typed struct — not real distinct fields. Reconstructed using the real handle
// (userConfigRef) and its real operator->() to reach the per-user apCONFIG for every GetValue<T> call.
//
// DEVIATION: each named-preset comparison chain (button/stick layout) decompiles as a manual
// byte-by-byte loop (compare, stop at NUL, break on mismatch) — the classic inlined-strcmp idiom.
// Reproduced as literal strcmp() calls; the resulting preset numbering is unchanged.
void haloENGINE_CONTROL::ApplySettings(bool isPrimary)
{
    if (!load_as_halo)
        return;

    int userIdx = isPrimary ? gsUserMng->PrimaryUserIdx() : gsUserMng->SecondaryUserIdx();

    ds_data::REF_TYPE<apCONFIG> userConfigRef = gsUserCfgMng->Get(userIdx);

    bool isSecondary = !isPrimary;
    // hcex_get_local_player_profile returns _player_ui_globals* (DB); the profile is its prefix. (2026-07-31)
    player_profile *profile = (player_profile *)hcex_get_local_player_profile(isSecondary);
    if (!profile)
    {
        _apLog("~HCEX,User,Error~Couldnt get HALO user profile for user <%d>", isSecondary);
        userConfigRef.Release(userConfigRef.m_pData);
        return;
    }

    apCONFIG *cfg = userConfigRef.operator->();

    bool invertLook = false;
    cfg->GetValue<bool>("Controls.InvertLook", &invertLook);
    profile->controller_settings.invert_look = invertLook;

    bool invertFlight = false;
    cfg->GetValue<bool>("Controls.InvertFlight", &invertFlight);
    profile->controller_settings.flight_stick_aircraft_controls = invertFlight;

    bool autocenter = false;
    cfg->GetValue<bool>("Controls.Autocenter", &autocenter);
    profile->controller_settings.autocenter = autocenter;

    int lookSensitivity = 3;
    cfg->GetValue<int>("Controls.LookSensitivity", &lookSensitivity);
    profile->controller_settings.look_sensitivity = (unsigned char)lookSensitivity;

    // DEVIATION: the decompiler writes the joystick-vibration setting straight into the global
    // hcex_off_vibration, not into the profile — reproduced as observed (not a reconstruction slip).
    bool joyVibrationOn = false;
    cfg->GetValue<bool>("Controls.JoyVibration", &joyVibrationOn);
    hcex_off_vibration = !joyVibrationOn;

    dsTSTRING<char> buttonLayout;
    buttonLayout.pBuffer = nullptr;
    buttonLayout.UnsafeInit("default", -1, 0);
    cfg->GetValue<dsTSTRING<char>>("Controls.ButtonLayout", &buttonLayout);
    buttonLayout.ToLower();
    const char *button = buttonLayout.pBuffer->str;

    if (strcmp(button, "default") == 0)
        profile->controller_settings.button_preset = _button_preset_standard;
    else if (strcmp(button, "southpaw") == 0)
        profile->controller_settings.button_preset = _button_preset_southpaw;
    else if (strcmp(button, "jumpy") == 0)
        profile->controller_settings.button_preset = _button_preset_jumpy;
    /* FIX: earlier reconstruction had boxer=3 / bumperjumper=4 swapped; the binary
     * (decompile @0x823C7F40: "boxer" branch assigns 4, "bumperjumper" assigns 3)
     * matches the DB enum _button_preset_boxer=4 / _button_preset_bumperjumper=3. */
    else if (strcmp(button, "boxer") == 0)
        profile->controller_settings.button_preset = _button_preset_boxer;
    else if (strcmp(button, "bumperjumper") == 0)
        profile->controller_settings.button_preset = _button_preset_bumperjumper;
    else if (strcmp(button, "greenthumb") == 0)
        profile->controller_settings.button_preset = _button_preset_greenthumb;
    else if (strcmp(button, "theduke") == 0)
        profile->controller_settings.button_preset = _button_preset_theduke;
    else if (strcmp(button, "hero") == 0)
        profile->controller_settings.button_preset = _button_preset_hero;
    else if (!IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0", "D:\\Projects\\code\\HCEX\\sources\\halo_engine_ctrl.cpp", 789, "");

    dsTSTRING<char> stickLayout;
    stickLayout.pBuffer = nullptr;
    stickLayout.UnsafeInit("default", -1, 0);
    cfg->GetValue<dsTSTRING<char>>("Controls.StickLayout", &stickLayout);
    stickLayout.ToLower();
    const char *stick = stickLayout.pBuffer->str;

    if (strcmp(stick, "default") == 0)
        profile->controller_settings.joystick_preset = _joystick_preset_standard;
    else if (strcmp(stick, "southpaw") == 0)
        profile->controller_settings.joystick_preset = _joystick_preset_south_paw;
    else if (strcmp(stick, "legacy") == 0)
        profile->controller_settings.joystick_preset = _joystick_preset_legacy;
    else if (strcmp(stick, "legacysouthpaw") == 0)
        profile->controller_settings.joystick_preset = _joystick_preset_legacy_south_paw;
    else if (!IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0", "D:\\Projects\\code\\HCEX\\sources\\halo_engine_ctrl.cpp", 804, "");

    set_local_player_controls_from_player_profile(isSecondary);

    if (--stickLayout.pBuffer->refCount == 0)
        dlFree(stickLayout.pBuffer);
    if (--buttonLayout.pBuffer->refCount == 0)
        dlFree(buttonLayout.pBuffer);
    userConfigRef.Release(userConfigRef.m_pData);
}
