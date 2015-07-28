// ----------------------------------------------------------------------------
// Controller.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "ModularDevice.h"
// #include "StandaloneInterface.h"
#include "EventController.h"
#include "BetterMap.h"
#include "TimerOne.h"
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
  // void executeStandaloneCallback();
  // bool getLedsPowered();
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
  // Standalone::StandaloneInterface standalone_interface_;
  MotorDrive motor_drive_;
  EventController::EventId waypoint_repeat_event_id_;
  bool waypoint_repeating_;
  void attachMotionInterrupt();
  void attachSoundInterrupt();
};

extern Controller controller;

#endif
