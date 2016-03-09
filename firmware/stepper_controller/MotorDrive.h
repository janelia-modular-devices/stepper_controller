// ----------------------------------------------------------------------------
// MotorDrive.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------

#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "ModularServer.h"
#include "TimerOne.h"
#include "Stepper.h"
#include "Array.h"
#include "Constants.h"


class MotorDrive
{
public:
  MotorDrive();
  ~MotorDrive();

  void setup();
  void update();

  void enable();
  void disable();
  bool isEnabled();

  void stop(unsigned int motor_index);
  void start(unsigned int motor_index);
  void stopAll();
  void startAll();
  bool areAnyRunning();
  bool isRunning(unsigned int motor_index);
  Array<bool,constants::MOTOR_COUNT> isRunningAll();

  void setSpeed();

  void setDirection(unsigned int motor_index, char dir);
  void setDirectionAll(Array<char, constants::MOTOR_COUNT> dir);

  long getCurrentPosition(unsigned int motor_index);
  Array<long, constants::MOTOR_COUNT> getCurrentPositionAll();
  void setCurrentPosition(unsigned int motor_index, long pos);
  void setCurrentPositionAll(Array<long, constants::MOTOR_COUNT> pos);

  long getTargetPosition(unsigned int motor_index);
  Array<long, constants::MOTOR_COUNT> getTargetPositionAll();
  void setTargetPosition(unsigned int motor_index, long pos);
  void setTargetPositionAll(Array<long, constants::MOTOR_COUNT> pos);

  int getCurrentWaypoint(unsigned int motor_index);
  Array<int, constants::MOTOR_COUNT> getCurrentWaypointAll();

  void goToNextWaypoint(unsigned int motor_index);
  void zero(unsigned int motor_index);
  void zeroAll();

private:
  Array<Stepper,constants::MOTOR_COUNT> steppers_;
  int enable_pin_;
  bool enabled_flag_;
};


inline void MotorDrive::update()
{
  if (enabled_flag_)
  {
    for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
    {
      steppers_[motor_index].updateDirPin();
      steppers_[motor_index].setStepPinHigh();
    }
    for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
    {
      steppers_[motor_index].setStepPinLow();
    }
  }
}

#endif
