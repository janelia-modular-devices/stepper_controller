// ----------------------------------------------------------------------------
// Callbacks.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "ArduinoJson.h"
#include "ModularServer.h"
#include "Constants.h"
#include "Globals.h"
#include "Controller.h"


namespace callbacks
{
void enableCallback();

void disableCallback();

void stopCallback();

void getControllerInfoCallback();

void moveCallback();

void playToneCallback();

void setToneFrequencyCallback();

void setToneDurationCallback();

void setWaypointCountCallback();

void setWaypointTravelDurationCallback();

void setPlayToneBeforeMoveCallback();

void setWaypointRepeatCallback();

void setWaypointRepeatPeriodCallback();

// EventController Callbacks
void waypointRepeatCallback(int index);

// ISR Callbacks
void motorDriveCallback();

void motionCallback();

void soundCallback();
}
#endif
