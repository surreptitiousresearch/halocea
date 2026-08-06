#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsEVENT_MGR.h"
// Boundary declarations for gsINP_KINECT::Load / Unload (0x826EE930 / 0x826EEA50): the physical
// Kinect device (speech-recognition control), the ds event system, and the interned command/rule
// name globals these two methods touch. All are engine internals reached through Load/Unload.

// Physical Kinect device — speech-recognition control surface.
struct inpKINECT {
    void InitSpeechRecognition(const dsTSTRING<wchar_t> *grammarFile);
    void DisableSpeechRecognition();
    void DisableSpeechRecognitionRule(const dsSTRID *rule);
    void EnableSpeechRecognitionRule(const dsSTRID *rule);
};

extern dsEVENT_MGR *gEventMgr;

// Event id for the "set Kinect microphone state" UI event, registered once by each of Load/Unload.
extern "C" int evid_uiSetKinectMicrophoneState;

// Interned microphone-state values signaled through the event.
extern "C" dsSTRID SPEECH_ST_INACTIVE; // Load: microphone inactive
extern "C" dsSTRID SPEECH_ST_NONE;     // Unload: microphone none

// Interned speech-recognition rule-set names.
extern "C" dsSTRID INP_KINECT_MAIN_RULE;   // gameplay grammar rule set
extern "C" dsSTRID INP_KINECT_SYSTEM_RULE; // system/menu grammar rule set

// Variadic logger. boundary.
void _apLog(const char *fmt, ...);
