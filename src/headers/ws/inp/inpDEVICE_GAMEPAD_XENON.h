#pragma once
// ws-engine inp: the Xenon (XInput) gamepad device. DB-verified layouts
// (types_members inpDEVICE_GAMEPAD_XENON / inpGAMEPAD / _XINPUT_GAMEPAD /
// _XINPUT_CAPABILITIES / _XINPUT_VIBRATION).

#include <stdint.h>
#include "inpDEVICE.h"

// XInput raw pad state (12 bytes).
typedef struct _XINPUT_GAMEPAD
{
    uint16_t wButtons;      /* 0x00 */
    uint8_t  bLeftTrigger;  /* 0x02 */
    uint8_t  bRightTrigger; /* 0x03 */
    int16_t          sThumbLX;      /* 0x04 */
    int16_t          sThumbLY;      /* 0x06 */
    int16_t          sThumbRX;      /* 0x08 */
    int16_t          sThumbRY;      /* 0x0A */
} _XINPUT_GAMEPAD;

typedef struct _XINPUT_VIBRATION
{
    uint16_t wLeftMotorSpeed;  /* 0x00 */
    uint16_t wRightMotorSpeed; /* 0x02 */
} _XINPUT_VIBRATION;

typedef struct _XINPUT_CAPABILITIES
{
    uint8_t   Type;      /* 0x00 */
    uint8_t   SubType;   /* 0x01 */
    uint16_t  Flags;     /* 0x02 */
    _XINPUT_GAMEPAD   Gamepad;   /* 0x04 */
    _XINPUT_VIBRATION Vibration; /* 0x10 */
} _XINPUT_CAPABILITIES;

// DB-verified (types_members inpGAMEPAD) -- size 116. Base _XINPUT_GAMEPAD @0.
typedef struct inpGAMEPAD
{
    _XINPUT_GAMEPAD      base;                    /* 0x00 (unnamed _XINPUT_GAMEPAD base) */
    uint16_t     wLastButtons;            /* 0x0C */
    unsigned char        _pad0E[2];               /* 0x0E */
    int                  bLastAnalogButtons[8];   /* 0x10 */
    uint16_t     wPressedButtons;         /* 0x30 */
    unsigned char        _pad32[2];               /* 0x32 */
    int                  bPressedAnalogButtons[8];/* 0x34 */
    _XINPUT_CAPABILITIES caps;                    /* 0x54 */
    int                  bInserted;               /* 0x68 */
    int                  bRemoved;                /* 0x6C */
    int                  bConnected;              /* 0x70 */
} inpGAMEPAD;

// DB-verified (types_members inpDEVICE_GAMEPAD_XENON) -- size 4572. Base inpDEVICE @0.
typedef struct inpDEVICE_GAMEPAD_XENON
{
    inpDEVICE  base;                 /* 0x0000 (4436) */
    inpGAMEPAD gamepad;              /* 0x1154 */
    float      currentDeadZone;      /* 0x11C8 */
    int        gamepadInstId;        /* 0x11CC */
    float      leftMotor;            /* 0x11D0 */
    float      rightMotor;           /* 0x11D4 */
    int        vibrationUpdateTimer; /* 0x11D8 */
} inpDEVICE_GAMEPAD_XENON;
