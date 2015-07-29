// ----------------------------------------------------------------------------
// Callbacks.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callbacks.h"

using namespace ArduinoJson::Parser;

namespace callbacks
{
// Callbacks must be non-blocking (avoid 'delay')
//
// modular_device.getParameterValue must be cast to either:
// char*
// long
// double
// bool
// JsonArray
// JsonObject
//
// For more info read about ArduinoJson v3 JsonParser JsonValues
//
// modular_device.getSavedVariableValue type must match the saved variable default type
// modular_device.setSavedVariableValue type must match the saved variable default type


// void getLedsPoweredCallback()
// {
//   bool leds_powered = controller.getLedsPowered();
//   modular_device.addBoolToResponse("leds_powered",leds_powered);
// }

void enableCallback()
{
  // long mfc = modular_device.getParameterValue(constants::mfc_parameter_name);
  // long percent = modular_device.getParameterValue(constants::percent_parameter_name);
  controller.enable();
}

void disableCallback()
{
  controller.disable();
}

void stopCallback()
{
  controller.stop();
}

void getControllerInfoCallback()
{
  modular_device.addKeyToResponse("controller_info");
  modular_device.startResponseObject();

  int motor_count = constants::MOTOR_COUNT;
  modular_device.addToResponse("motor_count",motor_count);

  bool is_enabled = controller.isEnabled();
  modular_device.addBoolToResponse("is_enabled",is_enabled);

  Array<bool,constants::MOTOR_COUNT> is_running = controller.isRunning();
  modular_device.addKeyToResponse("is_running");
  modular_device.startResponseArray();
  for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
  {
    modular_device.addBoolToResponse(is_running[motor_index]);
  }
  modular_device.stopResponseArray();

  Array<long,constants::MOTOR_COUNT> current_position = controller.getCurrentPosition();
  modular_device.addKeyToResponse("current_position");
  modular_device.startResponseArray();
  for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
  {
    modular_device.addToResponse(current_position[motor_index]);
  }
  modular_device.stopResponseArray();

  Array<long,constants::MOTOR_COUNT> target_position = controller.getTargetPosition();
  modular_device.addKeyToResponse("target_position");
  modular_device.startResponseArray();
  for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
  {
    modular_device.addToResponse(target_position[motor_index]);
  }
  modular_device.stopResponseArray();

  int tone_frequency;
  modular_device.getSavedVariableValue(constants::tone_frequency_parameter_name,tone_frequency);
  modular_device.addToResponse("tone_frequency",tone_frequency);

  int tone_duration;
  modular_device.getSavedVariableValue(constants::tone_duration_parameter_name,tone_duration);
  modular_device.addToResponse("tone_duration",tone_duration);

  bool play_tone_before_move;
  modular_device.getSavedVariableValue(constants::play_tone_before_move_parameter_name,play_tone_before_move);
  modular_device.addToResponse("play_tone_before_move",play_tone_before_move);

  constants::ModeType mode;
  modular_device.getSavedVariableValue(constants::mode_name,mode);
  if (mode == constants::WAYPOINT)
  {
    modular_device.addToResponse("mode","waypoint");

    modular_device.addKeyToResponse("waypoint_info");
    modular_device.startResponseObject();
    int waypoint_count;
    modular_device.getSavedVariableValue(constants::waypoint_count_parameter_name,waypoint_count);
    modular_device.addToResponse("waypoint_count",waypoint_count);
    int waypoint_travel_duration;
    modular_device.getSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
    modular_device.addToResponse("waypoint_travel_duration",waypoint_travel_duration);
    bool waypoint_repeat;
    modular_device.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
    modular_device.addToResponse("waypoint_repeat",waypoint_repeat);
    int waypoint_repeat_period;
    modular_device.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
    modular_device.addToResponse("waypoint_repeat_period",waypoint_repeat_period);
    modular_device.stopResponseObject();
  }

  modular_device.stopResponseObject();
}

void moveCallback()
{
  controller.move();
}

void playToneCallback()
{
  controller.playTone();
}

void setToneFrequencyCallback()
{
  long tone_frequency = modular_device.getParameterValue(constants::tone_frequency_parameter_name);
  modular_device.setSavedVariableValue(constants::tone_frequency_parameter_name,(int)tone_frequency);
}

void setToneDurationCallback()
{
  long tone_duration = modular_device.getParameterValue(constants::tone_duration_parameter_name);
  modular_device.setSavedVariableValue(constants::tone_duration_parameter_name,(int)tone_duration);
}

void setPlayToneBeforeMoveCallback()
{
  bool play_tone_before_move = modular_device.getParameterValue(constants::play_tone_before_move_parameter_name);
  modular_device.setSavedVariableValue(constants::play_tone_before_move_parameter_name,play_tone_before_move);
}

void setWaypointCountCallback()
{
  long waypoint_count = modular_device.getParameterValue(constants::waypoint_count_parameter_name);
  controller.setWaypointCount(waypoint_count);
}

void setWaypointTravelDurationCallback()
{
  long waypoint_travel_duration = modular_device.getParameterValue(constants::waypoint_travel_duration_parameter_name);
  bool waypoint_repeat;
  modular_device.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
  if (waypoint_repeat)
  {
    int waypoint_repeat_period;
    modular_device.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
    if (waypoint_travel_duration > waypoint_repeat_period)
    {
      modular_device.sendErrorResponse(constants::waypoint_parameter_error);
      return;
    }
  }
  controller.setWaypointTravelDuration(waypoint_travel_duration);
}

void setWaypointRepeatCallback()
{
  bool waypoint_repeat = modular_device.getParameterValue(constants::waypoint_repeat_parameter_name);
  if (waypoint_repeat)
  {
    int waypoint_travel_duration;
    modular_device.getSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
    int waypoint_repeat_period;
    modular_device.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
    if (waypoint_travel_duration > waypoint_repeat_period)
    {
      modular_device.sendErrorResponse(constants::waypoint_parameter_error);
      return;
    }
  }
  modular_device.setSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
}

void setWaypointRepeatPeriodCallback()
{
  long waypoint_repeat_period = modular_device.getParameterValue(constants::waypoint_repeat_period_parameter_name);
  bool waypoint_repeat;
  modular_device.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
  if (waypoint_repeat)
  {
    int waypoint_travel_duration;
    modular_device.getSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
    if (waypoint_travel_duration > waypoint_repeat_period)
    {
      modular_device.sendErrorResponse(constants::waypoint_parameter_error);
      return;
    }
  }
  modular_device.setSavedVariableValue(constants::waypoint_repeat_period_parameter_name,(int)waypoint_repeat_period);
}

// Standalone Callbacks
// void executeStandaloneCallbackCallback()
// {
//   controller.executeStandaloneCallback();
// }

// EventController Callbacks
void waypointRepeatCallback(int index)
{
  controller.goToNextWaypoint();
}

// ISR Callbacks
void motorDriveCallback()
{
  controller.motorDriveUpdate();
}

void motionCallback()
{
  controller.handleMotionInterrupt();
}

void soundCallback()
{
  controller.playTone();
}

}
