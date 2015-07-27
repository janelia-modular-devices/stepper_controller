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
  void handleWaypointInterrupt();
  // void executeStandaloneCallback();
  // bool getLedsPowered();
  void enable();
  void disable();
  bool isEnabled();
  void stop();
  Array<bool,constants::MOTOR_COUNT> isRunning();
  Array<long,constants::MOTOR_COUNT> getCurrentPosition();
  Array<long,constants::MOTOR_COUNT> getTargetPosition();
  void goToNextWaypoint();
  void setWaypointCount(int waypoint_count);
  void setWaypointTravelDuration(int waypoint_travel_duration);
  void playTone();
private:
  // Standalone::StandaloneInterface standalone_interface_;
  MotorDrive motor_drive_;
  void attachWaypointInterrupt();
  void attachToneInterrupt();
};

extern Controller controller;

inline void timerUpdate() {controller.motorDriveUpdate();}
inline void waypointISR() {controller.handleWaypointInterrupt();}
inline void toneISR() {controller.playTone();}
// inline void inlineGoToNextWaypoint(int dummy) {systemState.goToNextWaypoint();}

#endif
