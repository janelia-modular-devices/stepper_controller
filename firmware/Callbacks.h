// ----------------------------------------------------------------------------
// Callbacks.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "JsonParser.h"
#include "ModularDevice.h"
#include "Constants.h"
#include "Controller.h"


namespace callbacks
{
// void getLedsPoweredCallback();

void enableCallback();

void disableCallback();

void stopCallback();

void getStatusCallback();

void goToNextWaypointCallback();

void setWaypointCountCallback();

void setWaypointTravelDurationCallback();

void playToneCallback();

void setToneFrequencyCallback();

void setToneDurationCallback();

void setPlayToneBeforeMoveCallback();

// Standalone Callbacks
// void executeStandaloneCallbackCallback();

// EventController Callbacks
}
#endif
