// ----------------------------------------------------------------------------
// Stepper.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------

#ifndef STEPPER_H
#define STEPPER_H
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif
#include <util/atomic.h>
#include "ModularDevice.h"
#include "Constants.h"


class Stepper
{
public:
  Stepper();
  ~Stepper();

  void setup(uint8_t step_pin, uint8_t dir_pin);

  void start();
  void stop();
  bool isRunning();

  void disableOutputs();
  void enableOutputs();

  long getTargetPosition();
  void setTargetPosition(long position);
  long getCurrentPosition();
  void setCurrentPosition(long position);

  void setPinsInverted(bool direction, bool step);
  void setDirInverted();
  void setDirNormal();

  void goToNextWaypoint();
  int getCurrentWaypoint();
  void setCurrentWaypoint(int waypoint);

  void zero();

  void updateDirPin();
  void setStepPinHigh();
  void setStepPinLow();
private:
  uint8_t step_pin_;
  uint8_t dir_pin_;

  volatile bool dir_inverted_;
  volatile bool step_inverted_;

  uint8_t step_bit_mask_;
  uint8_t dir_bit_mask_;
  uint8_t step_port_;
  uint8_t dir_port_;
  volatile uint8_t *dir_port_reg_;
  volatile uint8_t *step_port_reg_;

  volatile bool running_;
  volatile long current_pos_;   // Steps
  volatile long target_pos_;    // Steps
  volatile int waypoint_;

};

inline void Stepper::updateDirPin()
{
  if (running_)
  {
    if (current_pos_ <= target_pos_)
    {
      if (dir_inverted_)
      {
        *dir_port_reg_ &= ~ dir_bit_mask_;
      }
      else
      {
        *dir_port_reg_ |= dir_bit_mask_;
      }
      current_pos_ += 1;
    }
    else if (current_pos_ > target_pos_)
    {
      if (dir_inverted_)
      {
        *dir_port_reg_ |= dir_bit_mask_;
      }
      else
      {
        *dir_port_reg_ &= ~ dir_bit_mask_;
      }
      current_pos_ -= 1;
    }
  }
}

inline void Stepper::setStepPinHigh()
{
  if (running_)
  {
    if (step_inverted_)
    {
      *step_port_reg_ &= ~step_bit_mask_;
    }
    else {
      *step_port_reg_ |= step_bit_mask_;
    }
  }
}

inline void Stepper::setStepPinLow()
{
  if (running_)
  {
    if (step_inverted_)
    {
      *step_port_reg_ |= step_bit_mask_;
    }
    else
    {
      *step_port_reg_ &= ~step_bit_mask_;
    }
    if (current_pos_ == target_pos_)
    {
      running_ = false;
      constants::ModeType mode;
      modular_device.getSavedVariableValue(constants::mode_name,mode);
      if (mode == constants::WAYPOINT)
      {
        waypoint_++;
        int waypoint_count;
        modular_device.getSavedVariableValue(constants::waypoint_count_parameter_name,waypoint_count);
        if (waypoint_ == waypoint_count)
        {
          waypoint_ = 0;
          setCurrentPosition(0);
        }
      }
    }
  }
}

#endif
