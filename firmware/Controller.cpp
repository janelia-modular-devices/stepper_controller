// ----------------------------------------------------------------------------
// Controller.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Controller.h"

// Controller::Controller() :
//   standalone_interface_(Standalone::StandaloneInterface(constants::display_serial,
//                                                         constants::enc_a_pin,
//                                                         constants::enc_b_pin,
//                                                         constants::enc_btn_pin,
//                                                         constants::enc_btn_int,
//                                                         constants::btn_pin,
//                                                         constants::btn_int,
//                                                         constants::led_pwr_pin,
//                                                         constants::standalone_update_period))
// {
// }

Controller::Controller()
{
}

void Controller::setup()
{
  EventController::event_controller.setup();
  Timer1.initialize();
  motor_drive_.setup();
  Timer1.attachInterrupt(timerUpdate);
  attachWaypointInterrupt();
  attachToneInterrupt();

  // Pin Setup
  // for (int mfc = 0; mfc < constants::MFC_COUNT; mfc++)
  // {
  //   pinMode(constants::mfc_purge_pins[mfc],INPUT);
  //   pinMode(constants::mfc_valve_off_pins[mfc],INPUT);
  // }
  // pinMode(constants::bnc_b_pin,OUTPUT);

  // Device Info
  modular_device.setName(constants::device_name);
  modular_device.setModelNumber(constants::model_number);
  modular_device.setFirmwareNumber(constants::firmware_number);

  // Saved Variables
  modular_device.createSavedVariable(constants::mode_name,
                                     constants::mode_default);
  modular_device.createSavedVariable(constants::waypoint_count_parameter_name,
                                     constants::waypoint_count_default);
  modular_device.createSavedVariable(constants::micro_steps_per_step_parameter_name,
                                     constants::micro_steps_per_step_default);
  modular_device.createSavedVariable(constants::waypoint_travel_duration_parameter_name,
                                     constants::waypoint_travel_duration_default);
  modular_device.createSavedVariable(constants::enable_polarity_name,
                                     constants::enable_polarity_default);
  modular_device.createSavedVariable(constants::tone_frequency_parameter_name,
                                     constants::tone_frequency_default);
  modular_device.createSavedVariable(constants::tone_duration_parameter_name,
                                     constants::tone_duration_default);
  modular_device.createSavedVariable(constants::play_tone_before_move_parameter_name,
                                     constants::play_tone_before_move_default);

  // Parameters
  ModularDevice::Parameter& waypoint_count_parameter = modular_device.createParameter(constants::waypoint_count_parameter_name);
  waypoint_count_parameter.setRange(constants::waypoint_count_min,constants::waypoint_count_max);

  ModularDevice::Parameter& waypoint_travel_duration_parameter = modular_device.createParameter(constants::waypoint_travel_duration_parameter_name);
  waypoint_travel_duration_parameter.setRange(constants::waypoint_travel_duration_min,constants::waypoint_travel_duration_max);
  waypoint_travel_duration_parameter.setUnits(constants::duration_parameter_units);

  ModularDevice::Parameter& tone_frequency_parameter = modular_device.createParameter(constants::tone_frequency_parameter_name);
  tone_frequency_parameter.setRange(constants::tone_frequency_min,constants::tone_frequency_max);
  tone_frequency_parameter.setUnits(constants::frequency_parameter_units);

  ModularDevice::Parameter& tone_duration_parameter = modular_device.createParameter(constants::tone_duration_parameter_name);
  tone_duration_parameter.setRange(constants::tone_duration_min,constants::tone_duration_max);
  tone_duration_parameter.setUnits(constants::duration_parameter_units);

  ModularDevice::Parameter& play_tone_before_move_parameter = modular_device.createParameter(constants::play_tone_before_move_parameter_name);
  play_tone_before_move_parameter.setTypeBool();

  // Methods
  // ModularDevice::Method& execute_standalone_callback_method = modular_device.createMethod(constants::execute_standalone_callback_method_name);
  // execute_standalone_callback_method.attachCallback(callbacks::executeStandaloneCallbackCallback);

  // ModularDevice::Method& get_leds_powered_method = modular_device.createMethod(constants::get_leds_powered_method_name);
  // get_leds_powered_method.attachCallback(callbacks::getLedsPoweredCallback);

  ModularDevice::Method& enable_method = modular_device.createMethod(constants::enable_method_name);
  enable_method.attachCallback(callbacks::enableCallback);

  ModularDevice::Method& disable_method = modular_device.createMethod(constants::disable_method_name);
  disable_method.attachCallback(callbacks::disableCallback);

  ModularDevice::Method& stop_method = modular_device.createMethod(constants::stop_method_name);
  stop_method.attachCallback(callbacks::stopCallback);

  ModularDevice::Method& get_status_method = modular_device.createMethod(constants::get_status_method_name);
  get_status_method.attachCallback(callbacks::getStatusCallback);

  ModularDevice::Method& go_to_next_waypoint_method = modular_device.createMethod(constants::go_to_next_waypoint_method_name);
  go_to_next_waypoint_method.attachCallback(callbacks::goToNextWaypointCallback);

  ModularDevice::Method& set_waypoint_count_method = modular_device.createMethod(constants::set_waypoint_count_method_name);
  set_waypoint_count_method.addParameter(waypoint_count_parameter);
  set_waypoint_count_method.attachCallback(callbacks::setWaypointCountCallback);

  ModularDevice::Method& set_waypoint_travel_duration_method = modular_device.createMethod(constants::set_waypoint_travel_duration_method_name);
  set_waypoint_travel_duration_method.addParameter(waypoint_travel_duration_parameter);
  set_waypoint_travel_duration_method.attachCallback(callbacks::setWaypointTravelDurationCallback);

  ModularDevice::Method& play_tone_method = modular_device.createMethod(constants::play_tone_method_name);
  play_tone_method.attachCallback(callbacks::playToneCallback);

  ModularDevice::Method& set_tone_frequency_method = modular_device.createMethod(constants::set_tone_frequency_method_name);
  set_tone_frequency_method.addParameter(tone_frequency_parameter);
  set_tone_frequency_method.attachCallback(callbacks::setToneFrequencyCallback);

  ModularDevice::Method& set_tone_duration_method = modular_device.createMethod(constants::set_tone_duration_method_name);
  set_tone_duration_method.addParameter(tone_duration_parameter);
  set_tone_duration_method.attachCallback(callbacks::setToneDurationCallback);

  ModularDevice::Method& set_play_tone_before_move_method = modular_device.createMethod(constants::set_play_tone_before_move_method_name);
  set_play_tone_before_move_method.addParameter(play_tone_before_move_parameter);
  set_play_tone_before_move_method.attachCallback(callbacks::setPlayToneBeforeMoveCallback);

  // Start Server
  modular_device.startServer(constants::baudrate);

  // Standalone Interface
  // standalone_interface_.setup(constants::frame_name_array,constants::FRAME_COUNT);

  // Display Labels

  // Display Variables

  // Interactive Variables

  // All Frames

  // Frame 0
  // int frame = 0;

  // Frame 1
  // frame = 1;


  // Enable Standalone Interface
  // standalone_interface_.enable();
}

void Controller::update()
{
  modular_device.handleServerRequests();
  // bool updated = standalone_interface_.update();
  // if (updated)
  // {
  //   updateDisplayVariables();
  // }
}

void Controller::motorDriveUpdate()
{
  motor_drive_.update();
}

void Controller::handleWaypointInterrupt()
{
  goToNextWaypoint();
}

// void Controller::executeStandaloneCallback()
// {
//   standalone_interface_.executeCurrentFrameCallback();
// }

// bool Controller::getLedsPowered()
// {
//   return digitalRead(constants::led_pwr_pin) == HIGH;
// }

void Controller::enable()
{
  motor_drive_.enable();
}

void Controller::disable()
{
  motor_drive_.disable();
}

bool Controller::isEnabled()
{
  return motor_drive_.isEnabled();
}

void Controller::stop()
{
  motor_drive_.stopAll();
}

Array<bool,constants::MOTOR_COUNT> Controller::isRunning()
{
  return motor_drive_.isRunningAll();
}

Array<long,constants::MOTOR_COUNT> Controller::getCurrentPosition()
{
  return motor_drive_.getCurrentPositionAll();
}

Array<long,constants::MOTOR_COUNT> Controller::getTargetPosition()
{
  return motor_drive_.getTargetPositionAll();
}

void Controller::goToNextWaypoint()
{
  if (!isEnabled())
  {
    enable();
  }
  bool play_tone_before_move;
  modular_device.getSavedVariableValue(constants::play_tone_before_move_parameter_name,play_tone_before_move);
  if (play_tone_before_move)
  {
    playTone();
  }
  motor_drive_.goToNextWaypoint(0);
}

void Controller::setWaypointCount(int waypoint_count)
{
  motor_drive_.stopAll();
  motor_drive_.zeroAll();
  modular_device.setSavedVariableValue(constants::waypoint_count_parameter_name,waypoint_count);
  motor_drive_.setSpeed();
}

void Controller::setWaypointTravelDuration(int waypoint_travel_duration)
{
  motor_drive_.stopAll();
  motor_drive_.zeroAll();
  modular_device.setSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
  motor_drive_.setSpeed();
}

void Controller::playTone()
{
  int tone_frequency;
  modular_device.getSavedVariableValue(constants::tone_frequency_parameter_name,tone_frequency);

  int tone_duration;
  modular_device.getSavedVariableValue(constants::tone_duration_parameter_name,tone_duration);

  tone(constants::speaker_pin,tone_frequency,tone_duration);
}

void Controller::attachWaypointInterrupt()
{
  pinMode(constants::waypoint_interrupt_pin, INPUT);
  digitalWrite(constants::waypoint_interrupt_pin, HIGH);
  int waypoint_interrupt_mode = constants::waypoint_interrupt_mode_default;
  attachInterrupt(constants::waypoint_interrupt_number,waypointISR,waypoint_interrupt_mode);
}

void Controller::attachToneInterrupt() {
  pinMode(constants::tone_interrupt_pin, INPUT);
  digitalWrite(constants::tone_interrupt_pin, HIGH);
  int tone_interrupt_mode = constants::tone_interrupt_mode_default;
  attachInterrupt(constants::tone_interrupt_number,toneISR,tone_interrupt_mode);
}

Controller controller;


