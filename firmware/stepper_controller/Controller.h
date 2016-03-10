// ----------------------------------------------------------------------------
// Controller.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "ModularServer.h"
#include "EventController.h"
#include "BetterMap.h"
#include "TimerOne.h"
#include "Globals.h"
#include "Constants.h"
#include "Callbacks.h"
#include "MotorDrive.h"


class Controller
{
public:
  Controller();
  void setup();
  void update();

  void motorDriveUpdate();
  void handleMotionInterrupt();
  void enable();
  void disable();
  bool isEnabled();
  void stop();
  void move();
  Array<bool,constants::MOTOR_COUNT> isRunning();
  Array<long,constants::MOTOR_COUNT> getCurrentPosition();
  Array<long,constants::MOTOR_COUNT> getTargetPosition();
  void playTone();
  void goToNextWaypoint();
  void setWaypointCount(int waypoint_count);
  void setWaypointTravelDuration(int waypoint_travel_duration);
  void startWaypointRepeat();
  void stopWaypointRepeat();
private:
  SavedVariable saved_variables_[constants::SAVED_VARIABLE_COUNT_MAX];
  ModularDevice::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  ModularDevice::Method methods_[constants::METHOD_COUNT_MAX];

  MotorDrive motor_drive_;
  EventController::EventId waypoint_repeat_event_id_;
  bool waypoint_repeating_;
  void attachMotionInterrupts();
  void attachSoundInterrupts();
};

extern Controller controller;

#endif
