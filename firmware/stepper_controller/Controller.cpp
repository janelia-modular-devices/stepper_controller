// ----------------------------------------------------------------------------
// Controller.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Controller.h"

Controller::Controller()
{
  waypoint_repeating_ = false;
}

void Controller::setup()
{
  EventController::event_controller.setup();
  Timer1.initialize();
  motor_drive_.setup();
  Timer1.attachInterrupt(callbacks::motorDriveCallback);
  attachMotionInterrupts();
  attachSoundInterrupts();

  // Pin Setup

  // Device Info
  globals::modular_server.setName(constants::device_name);
  globals::modular_server.setModelNumber(constants::model_number);
  globals::modular_server.setFirmwareVersion(constants::firmware_major,constants::firmware_minor,constants::firmware_patch);

  // Add Server Streams
  globals::modular_server.addServerStream(Serial);

  // Set Storage
  globals::modular_server.setSavedVariableStorage(saved_variables_);
  globals::modular_server.setParameterStorage(parameters_);
  globals::modular_server.setMethodStorage(methods_);

  // Saved Variables
  globals::modular_server.createSavedVariable(constants::mode_name,
                                              constants::mode_default);
  globals::modular_server.createSavedVariable(constants::waypoint_count_parameter_name,
                                              constants::waypoint_count_default);
  globals::modular_server.createSavedVariable(constants::micro_steps_per_step_parameter_name,
                                              constants::micro_steps_per_step_default);
  globals::modular_server.createSavedVariable(constants::waypoint_travel_duration_parameter_name,
                                              constants::waypoint_travel_duration_default);
  globals::modular_server.createSavedVariable(constants::enable_polarity_name,
                                              constants::enable_polarity_default);
  globals::modular_server.createSavedVariable(constants::tone_frequency_parameter_name,
                                              constants::tone_frequency_default);
  globals::modular_server.createSavedVariable(constants::tone_duration_parameter_name,
                                              constants::tone_duration_default);
  globals::modular_server.createSavedVariable(constants::play_tone_before_move_parameter_name,
                                              constants::play_tone_before_move_default);
  globals::modular_server.createSavedVariable(constants::waypoint_repeat_parameter_name,
                                              constants::waypoint_repeat_default);
  globals::modular_server.createSavedVariable(constants::waypoint_repeat_period_parameter_name,
                                              constants::waypoint_repeat_period_default);

  // Parameters
  ModularDevice::Parameter & waypoint_count_parameter = globals::modular_server.createParameter(constants::waypoint_count_parameter_name);
  waypoint_count_parameter.setRange(constants::waypoint_count_min,constants::waypoint_count_max);

  ModularDevice::Parameter & waypoint_travel_duration_parameter = globals::modular_server.createParameter(constants::waypoint_travel_duration_parameter_name);
  waypoint_travel_duration_parameter.setRange(constants::waypoint_travel_duration_min,constants::waypoint_travel_duration_max);
  waypoint_travel_duration_parameter.setUnits(constants::duration_parameter_units);

  ModularDevice::Parameter & tone_frequency_parameter = globals::modular_server.createParameter(constants::tone_frequency_parameter_name);
  tone_frequency_parameter.setRange(constants::tone_frequency_min,constants::tone_frequency_max);
  tone_frequency_parameter.setUnits(constants::frequency_parameter_units);

  ModularDevice::Parameter & tone_duration_parameter = globals::modular_server.createParameter(constants::tone_duration_parameter_name);
  tone_duration_parameter.setRange(constants::tone_duration_min,constants::tone_duration_max);
  tone_duration_parameter.setUnits(constants::duration_parameter_units);

  ModularDevice::Parameter & play_tone_before_move_parameter = globals::modular_server.createParameter(constants::play_tone_before_move_parameter_name);
  play_tone_before_move_parameter.setTypeBool();

  ModularDevice::Parameter & waypoint_repeat_parameter = globals::modular_server.createParameter(constants::waypoint_repeat_parameter_name);
  waypoint_repeat_parameter.setTypeBool();

  ModularDevice::Parameter & waypoint_repeat_period_parameter = globals::modular_server.createParameter(constants::waypoint_repeat_period_parameter_name);
  waypoint_repeat_period_parameter.setRange(constants::waypoint_repeat_period_min,constants::waypoint_repeat_period_max);
  waypoint_repeat_period_parameter.setUnits(constants::duration_parameter_units);

  // Methods
  ModularDevice::Method & enable_method = globals::modular_server.createMethod(constants::enable_method_name);
  enable_method.attachCallback(callbacks::enableCallback);

  ModularDevice::Method & disable_method = globals::modular_server.createMethod(constants::disable_method_name);
  disable_method.attachCallback(callbacks::disableCallback);

  ModularDevice::Method & stop_method = globals::modular_server.createMethod(constants::stop_method_name);
  stop_method.attachCallback(callbacks::stopCallback);

  ModularDevice::Method & get_controller_info_method = globals::modular_server.createMethod(constants::get_controller_info_method_name);
  get_controller_info_method.attachCallback(callbacks::getControllerInfoCallback);
  get_controller_info_method.setReturnTypeObject();

  ModularDevice::Method & move_method = globals::modular_server.createMethod(constants::move_method_name);
  move_method.attachCallback(callbacks::moveCallback);

  ModularDevice::Method & play_tone_method = globals::modular_server.createMethod(constants::play_tone_method_name);
  play_tone_method.attachCallback(callbacks::playToneCallback);

  ModularDevice::Method & set_tone_frequency_method = globals::modular_server.createMethod(constants::set_tone_frequency_method_name);
  set_tone_frequency_method.addParameter(tone_frequency_parameter);
  set_tone_frequency_method.attachCallback(callbacks::setToneFrequencyCallback);

  ModularDevice::Method & set_tone_duration_method = globals::modular_server.createMethod(constants::set_tone_duration_method_name);
  set_tone_duration_method.addParameter(tone_duration_parameter);
  set_tone_duration_method.attachCallback(callbacks::setToneDurationCallback);

  ModularDevice::Method & set_play_tone_before_move_method = globals::modular_server.createMethod(constants::set_play_tone_before_move_method_name);
  set_play_tone_before_move_method.addParameter(play_tone_before_move_parameter);
  set_play_tone_before_move_method.attachCallback(callbacks::setPlayToneBeforeMoveCallback);

  ModularDevice::Method & set_waypoint_count_method = globals::modular_server.createMethod(constants::set_waypoint_count_method_name);
  set_waypoint_count_method.addParameter(waypoint_count_parameter);
  set_waypoint_count_method.attachCallback(callbacks::setWaypointCountCallback);

  ModularDevice::Method & set_waypoint_travel_duration_method = globals::modular_server.createMethod(constants::set_waypoint_travel_duration_method_name);
  set_waypoint_travel_duration_method.addParameter(waypoint_travel_duration_parameter);
  set_waypoint_travel_duration_method.attachCallback(callbacks::setWaypointTravelDurationCallback);

  ModularDevice::Method & set_waypoint_repeat_method = globals::modular_server.createMethod(constants::set_waypoint_repeat_method_name);
  set_waypoint_repeat_method.addParameter(waypoint_repeat_parameter);
  set_waypoint_repeat_method.attachCallback(callbacks::setWaypointRepeatCallback);

  ModularDevice::Method & set_waypoint_repeat_period_method = globals::modular_server.createMethod(constants::set_waypoint_repeat_period_method_name);
  set_waypoint_repeat_period_method.addParameter(waypoint_repeat_period_parameter);
  set_waypoint_repeat_period_method.attachCallback(callbacks::setWaypointRepeatPeriodCallback);

  // Setup Streams
  Serial.begin(constants::baudrate);

  // Start Server
  globals::modular_server.startServer();
}

void Controller::update()
{
  globals::modular_server.handleServerRequests();
}

void Controller::motorDriveUpdate()
{
  motor_drive_.update();
}

void Controller::handleMotionInterrupt()
{
  if (!isEnabled())
  {
    enable();
  }
  constants::ModeType mode;
  globals::modular_server.getSavedVariableValue(constants::mode_name,mode);
  if (mode == constants::WAYPOINT)
  {
    bool waypoint_repeat;
    globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
    if (!waypoint_repeat)
    {
      goToNextWaypoint();
    }
    else if (!waypoint_repeating_)
    {
      startWaypointRepeat();
    }
    else
    {
      stopWaypointRepeat();
    }
  }
}

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
  stopWaypointRepeat();
  motor_drive_.stopAll();
}

void Controller::move()
{
  if (!isEnabled())
  {
    enable();
  }
  constants::ModeType mode;
  globals::modular_server.getSavedVariableValue(constants::mode_name,mode);
  if (mode == constants::WAYPOINT)
  {
    bool waypoint_repeat;
    globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
    if (!waypoint_repeat)
    {
      goToNextWaypoint();
    }
    else
    {
      startWaypointRepeat();
    }
  }
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

void Controller::playTone()
{
  int tone_frequency;
  globals::modular_server.getSavedVariableValue(constants::tone_frequency_parameter_name,tone_frequency);

  int tone_duration;
  globals::modular_server.getSavedVariableValue(constants::tone_duration_parameter_name,tone_duration);

  tone(constants::speaker_pin,tone_frequency,tone_duration);
}

void Controller::goToNextWaypoint()
{
  bool play_tone_before_move;
  globals::modular_server.getSavedVariableValue(constants::play_tone_before_move_parameter_name,play_tone_before_move);
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
  globals::modular_server.setSavedVariableValue(constants::waypoint_count_parameter_name,waypoint_count);
  motor_drive_.setSpeed();
}

void Controller::setWaypointTravelDuration(int waypoint_travel_duration)
{
  motor_drive_.stopAll();
  motor_drive_.zeroAll();
  globals::modular_server.setSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
  motor_drive_.setSpeed();
}

void Controller::startWaypointRepeat()
{
  if (!waypoint_repeating_)
  {
    waypoint_repeating_ = true;
    int waypoint_repeat_period;
    globals::modular_server.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
    waypoint_repeat_event_id_ = EventController::event_controller.addInfiniteRecurringEventUsingDelay(callbacks::waypointRepeatCallback,0,waypoint_repeat_period);
  }
}

void Controller::stopWaypointRepeat()
{
  if (waypoint_repeating_)
  {
    waypoint_repeating_ = false;
    EventController::event_controller.removeEvent(waypoint_repeat_event_id_);
  }
}

void Controller::attachMotionInterrupts()
{
  pinMode(constants::motion_switch_interrupt_pin, INPUT);
  digitalWrite(constants::motion_switch_interrupt_pin, HIGH);
  int motion_switch_interrupt_mode = constants::motion_switch_interrupt_mode_default;
  attachInterrupt(constants::motion_switch_interrupt_number,callbacks::motionCallback,motion_switch_interrupt_mode);

  pinMode(constants::motion_bnc_interrupt_pin, INPUT);
  digitalWrite(constants::motion_bnc_interrupt_pin, HIGH);
  int motion_bnc_interrupt_mode = constants::motion_bnc_interrupt_mode_default;
  attachInterrupt(constants::motion_bnc_interrupt_number,callbacks::motionCallback,motion_bnc_interrupt_mode);
}

void Controller::attachSoundInterrupts()
{
  pinMode(constants::sound_switch_interrupt_pin, INPUT);
  digitalWrite(constants::sound_switch_interrupt_pin, HIGH);
  int sound_switch_interrupt_mode = constants::sound_switch_interrupt_mode_default;
  attachInterrupt(constants::sound_switch_interrupt_number,callbacks::soundCallback,sound_switch_interrupt_mode);

  pinMode(constants::sound_bnc_interrupt_pin, INPUT);
  digitalWrite(constants::sound_bnc_interrupt_pin, HIGH);
  int sound_bnc_interrupt_mode = constants::sound_bnc_interrupt_mode_default;
  attachInterrupt(constants::sound_bnc_interrupt_number,callbacks::soundCallback,sound_bnc_interrupt_mode);
}

Controller controller;
