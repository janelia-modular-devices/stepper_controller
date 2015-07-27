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
//   boolean leds_powered = controller.getLedsPowered();
//   modular_device.addBooleanToResponse("leds_powered",leds_powered);
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

void getStatusCallback()
{
  modular_device.addKeyToResponse("status");
  modular_device.startResponseObject();

  int motor_count = constants::MOTOR_COUNT;
  modular_device.addToResponse("motor_count",motor_count);

  bool is_enabled = controller.isEnabled();
  modular_device.addBooleanToResponse("is_enabled",is_enabled);

  Array<bool,constants::MOTOR_COUNT> is_running = controller.isRunning();
  modular_device.addKeyToResponse("is_running");
  modular_device.startResponseArray();
  for (int motor_index=0; motor_index<constants::MOTOR_COUNT; motor_index++)
  {
    modular_device.addBooleanToResponse(is_running[motor_index]);
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
    modular_device.stopResponseObject();
  }

  int tone_frequency;
  modular_device.getSavedVariableValue(constants::tone_frequency_parameter_name,tone_frequency);
  modular_device.addToResponse("tone_frequency",tone_frequency);

  int tone_duration;
  modular_device.getSavedVariableValue(constants::tone_duration_parameter_name,tone_duration);
  modular_device.addToResponse("tone_duration",tone_duration);

  modular_device.stopResponseObject();
}

void goToNextWaypointCallback()
{
  controller.goToNextWaypoint();
}

void setWaypointCountCallback()
{
  long waypoint_count = modular_device.getParameterValue(constants::waypoint_count_parameter_name);
  controller.setWaypointCount(waypoint_count);
}

void setWaypointTravelDurationCallback()
{
  long waypoint_travel_duration = modular_device.getParameterValue(constants::waypoint_travel_duration_parameter_name);
  controller.setWaypointTravelDuration(waypoint_travel_duration);
}

void playToneCallback()
{
  controller.playTone();
}

void setToneFrequencyCallback()
{
  long tone_frequency = modular_device.getParameterValue(constants::tone_frequency_parameter_name);
  modular_device.setSavedVariableValue(constants::tone_frequency_parameter_name,tone_frequency);
}

void setToneDurationCallback()
{
  long tone_duration = modular_device.getParameterValue(constants::tone_duration_parameter_name);
  modular_device.setSavedVariableValue(constants::tone_duration_parameter_name,tone_duration);
}

// Standalone Callbacks
// void executeStandaloneCallbackCallback()
// {
//   controller.executeStandaloneCallback();
// }

// EventController Callbacks

}
