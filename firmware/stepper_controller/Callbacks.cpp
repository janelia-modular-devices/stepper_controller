// ----------------------------------------------------------------------------
// Callbacks.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callbacks.h"


namespace callbacks
{
// Callbacks must be non-blocking (avoid 'delay')
//
// modular_server.getParameterValue must be cast to either:
// const char *
// long
// double
// bool
// ArduinoJson::JsonArray &
// ArduinoJson::JsonObject &
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server.getSavedVariableValue type must match the saved variable default type
// modular_server.setSavedVariableValue type must match the saved variable default type


void enableCallback()
{
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
  globals::modular_server.writeResultKeyToResponse();
  globals::modular_server.beginResponseObject();

  int motor_count = constants::MOTOR_COUNT;
  globals::modular_server.writeToResponse("motor_count",motor_count);

  bool is_enabled = controller.isEnabled();
  globals::modular_server.writeToResponse("is_enabled",is_enabled);

  Array<bool,constants::MOTOR_COUNT> is_running = controller.isRunning();
  globals::modular_server.writeKeyToResponse("is_running");
  globals::modular_server.beginResponseArray();
  for (int motor_index=0; motor_index<constants::MOTOR_COUNT; ++motor_index)
  {
    globals::modular_server.writeToResponse(is_running[motor_index]);
  }
  globals::modular_server.endResponseArray();

  Array<long,constants::MOTOR_COUNT> current_position = controller.getCurrentPosition();
  globals::modular_server.writeKeyToResponse("current_position");
  globals::modular_server.beginResponseArray();
  for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
  {
    globals::modular_server.writeToResponse(current_position[motor_index]);
  }
  globals::modular_server.endResponseArray();

  Array<long,constants::MOTOR_COUNT> target_position = controller.getTargetPosition();
  globals::modular_server.writeKeyToResponse("target_position");
  globals::modular_server.beginResponseArray();
  for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
  {
    globals::modular_server.writeToResponse(target_position[motor_index]);
  }
  globals::modular_server.endResponseArray();

  int tone_frequency;
  globals::modular_server.getSavedVariableValue(constants::tone_frequency_parameter_name,tone_frequency);
  globals::modular_server.writeToResponse("tone_frequency",tone_frequency);

  int tone_duration;
  globals::modular_server.getSavedVariableValue(constants::tone_duration_parameter_name,tone_duration);
  globals::modular_server.writeToResponse("tone_duration",tone_duration);

  bool play_tone_before_move;
  globals::modular_server.getSavedVariableValue(constants::play_tone_before_move_parameter_name,play_tone_before_move);
  globals::modular_server.writeToResponse("play_tone_before_move",play_tone_before_move);

  constants::ModeType mode;
  globals::modular_server.getSavedVariableValue(constants::mode_name,mode);
  if (mode == constants::WAYPOINT)
  {
    globals::modular_server.writeToResponse("mode","waypoint");

    globals::modular_server.writeKeyToResponse("waypoint_info");
    globals::modular_server.beginResponseObject();
    int waypoint_count;
    globals::modular_server.getSavedVariableValue(constants::waypoint_count_parameter_name,waypoint_count);
    globals::modular_server.writeToResponse("waypoint_count",waypoint_count);
    int waypoint_travel_duration;
    globals::modular_server.getSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
    globals::modular_server.writeToResponse("waypoint_travel_duration",waypoint_travel_duration);
    bool waypoint_repeat;
    globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
    globals::modular_server.writeToResponse("waypoint_repeat",waypoint_repeat);
    int waypoint_repeat_period;
    globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
    globals::modular_server.writeToResponse("waypoint_repeat_period",waypoint_repeat_period);
    globals::modular_server.endResponseObject();
  }

  globals::modular_server.endResponseObject();
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
  long tone_frequency = globals::modular_server.getParameterValue(constants::tone_frequency_parameter_name);
  globals::modular_server.setSavedVariableValue(constants::tone_frequency_parameter_name,(int)tone_frequency);
}

void setToneDurationCallback()
{
  long tone_duration = globals::modular_server.getParameterValue(constants::tone_duration_parameter_name);
  globals::modular_server.setSavedVariableValue(constants::tone_duration_parameter_name,(int)tone_duration);
}

void setPlayToneBeforeMoveCallback()
{
  bool play_tone_before_move = globals::modular_server.getParameterValue(constants::play_tone_before_move_parameter_name);
  globals::modular_server.setSavedVariableValue(constants::play_tone_before_move_parameter_name,play_tone_before_move);
}

void setWaypointCountCallback()
{
  long waypoint_count = globals::modular_server.getParameterValue(constants::waypoint_count_parameter_name);
  controller.setWaypointCount(waypoint_count);
}

void setWaypointTravelDurationCallback()
{
  long waypoint_travel_duration = globals::modular_server.getParameterValue(constants::waypoint_travel_duration_parameter_name);
  bool waypoint_repeat;
  globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
  if (waypoint_repeat)
  {
    int waypoint_repeat_period;
    globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
    if (waypoint_travel_duration > waypoint_repeat_period)
    {
      globals::modular_server.sendErrorResponse(constants::waypoint_parameter_error);
      return;
    }
  }
  controller.setWaypointTravelDuration(waypoint_travel_duration);
}

void setWaypointRepeatCallback()
{
  bool waypoint_repeat = globals::modular_server.getParameterValue(constants::waypoint_repeat_parameter_name);
  if (waypoint_repeat)
  {
    int waypoint_travel_duration;
    globals::modular_server.getSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
    int waypoint_repeat_period;
    globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
    if (waypoint_travel_duration > waypoint_repeat_period)
    {
      globals::modular_server.sendErrorResponse(constants::waypoint_parameter_error);
      return;
    }
  }
  globals::modular_server.setSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
}

void setWaypointRepeatPeriodCallback()
{
  long waypoint_repeat_period = globals::modular_server.getParameterValue(constants::waypoint_repeat_period_parameter_name);
  bool waypoint_repeat;
  globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
  if (waypoint_repeat)
  {
    int waypoint_travel_duration;
    globals::modular_server.getSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
    if (waypoint_travel_duration > waypoint_repeat_period)
    {
      globals::modular_server.sendErrorResponse(constants::waypoint_parameter_error);
      return;
    }
  }
  globals::modular_server.setSavedVariableValue(constants::waypoint_repeat_period_parameter_name,(int)waypoint_repeat_period);
}

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
