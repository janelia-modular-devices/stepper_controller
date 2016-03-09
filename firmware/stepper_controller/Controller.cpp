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
  modular_server_.setName(constants::device_name);
  modular_server_.setModelNumber(constants::model_number);
  modular_server_.setFirmwareVersion(constants::firmware_major,constants::firmware_minor,constants::firmware_patch);

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Storage
  modular_server_.setSavedVariableStorage(saved_variables_);
  modular_server_.setParameterStorage(parameters_);
  modular_server_.setMethodStorage(methods_);

  // Saved Variables
  modular_server_.createSavedVariable(constants::mode_name,
                                      constants::mode_default);
  modular_server_.createSavedVariable(constants::waypoint_count_parameter_name,
                                      constants::waypoint_count_default);
  modular_server_.createSavedVariable(constants::micro_steps_per_step_parameter_name,
                                      constants::micro_steps_per_step_default);
  modular_server_.createSavedVariable(constants::waypoint_travel_duration_parameter_name,
                                      constants::waypoint_travel_duration_default);
  modular_server_.createSavedVariable(constants::enable_polarity_name,
                                      constants::enable_polarity_default);
  modular_server_.createSavedVariable(constants::tone_frequency_parameter_name,
                                      constants::tone_frequency_default);
  modular_server_.createSavedVariable(constants::tone_duration_parameter_name,
                                      constants::tone_duration_default);
  modular_server_.createSavedVariable(constants::play_tone_before_move_parameter_name,
                                      constants::play_tone_before_move_default);
  modular_server_.createSavedVariable(constants::waypoint_repeat_parameter_name,
                                      constants::waypoint_repeat_default);
  modular_server_.createSavedVariable(constants::waypoint_repeat_period_parameter_name,
                                      constants::waypoint_repeat_period_default);

  // Parameters
  ModularDevice::Parameter& waypoint_count_parameter = modular_server_.createParameter(constants::waypoint_count_parameter_name);
  waypoint_count_parameter.setRange(constants::waypoint_count_min,constants::waypoint_count_max);

  ModularDevice::Parameter& waypoint_travel_duration_parameter = modular_server_.createParameter(constants::waypoint_travel_duration_parameter_name);
  waypoint_travel_duration_parameter.setRange(constants::waypoint_travel_duration_min,constants::waypoint_travel_duration_max);
  waypoint_travel_duration_parameter.setUnits(constants::duration_parameter_units);

  ModularDevice::Parameter& tone_frequency_parameter = modular_server_.createParameter(constants::tone_frequency_parameter_name);
  tone_frequency_parameter.setRange(constants::tone_frequency_min,constants::tone_frequency_max);
  tone_frequency_parameter.setUnits(constants::frequency_parameter_units);

  ModularDevice::Parameter& tone_duration_parameter = modular_server_.createParameter(constants::tone_duration_parameter_name);
  tone_duration_parameter.setRange(constants::tone_duration_min,constants::tone_duration_max);
  tone_duration_parameter.setUnits(constants::duration_parameter_units);

  ModularDevice::Parameter& play_tone_before_move_parameter = modular_server_.createParameter(constants::play_tone_before_move_parameter_name);
  play_tone_before_move_parameter.setTypeBool();

  ModularDevice::Parameter& waypoint_repeat_parameter = modular_server_.createParameter(constants::waypoint_repeat_parameter_name);
  waypoint_repeat_parameter.setTypeBool();

  ModularDevice::Parameter& waypoint_repeat_period_parameter = modular_server_.createParameter(constants::waypoint_repeat_period_parameter_name);
  waypoint_repeat_period_parameter.setRange(constants::waypoint_repeat_period_min,constants::waypoint_repeat_period_max);
  waypoint_repeat_period_parameter.setUnits(constants::duration_parameter_units);

  // Methods
  ModularDevice::Method& enable_method = modular_server_.createMethod(constants::enable_method_name);
  enable_method.attachCallback(callbacks::enableCallback);

  ModularDevice::Method& disable_method = modular_server_.createMethod(constants::disable_method_name);
  disable_method.attachCallback(callbacks::disableCallback);

  ModularDevice::Method& stop_method = modular_server_.createMethod(constants::stop_method_name);
  stop_method.attachCallback(callbacks::stopCallback);

  ModularDevice::Method& get_controller_info_method = modular_server_.createMethod(constants::get_controller_info_method_name);
  get_controller_info_method.attachCallback(callbacks::getControllerInfoCallback);
  get_controller_info_method.setReturnTypeObject();

  ModularDevice::Method& move_method = modular_server_.createMethod(constants::move_method_name);
  move_method.attachCallback(callbacks::moveCallback);

  ModularDevice::Method& play_tone_method = modular_server_.createMethod(constants::play_tone_method_name);
  play_tone_method.attachCallback(callbacks::playToneCallback);

  ModularDevice::Method& set_tone_frequency_method = modular_server_.createMethod(constants::set_tone_frequency_method_name);
  set_tone_frequency_method.addParameter(tone_frequency_parameter);
  set_tone_frequency_method.attachCallback(callbacks::setToneFrequencyCallback);

  ModularDevice::Method& set_tone_duration_method = modular_server_.createMethod(constants::set_tone_duration_method_name);
  set_tone_duration_method.addParameter(tone_duration_parameter);
  set_tone_duration_method.attachCallback(callbacks::setToneDurationCallback);

  ModularDevice::Method& set_play_tone_before_move_method = modular_server_.createMethod(constants::set_play_tone_before_move_method_name);
  set_play_tone_before_move_method.addParameter(play_tone_before_move_parameter);
  set_play_tone_before_move_method.attachCallback(callbacks::setPlayToneBeforeMoveCallback);

  ModularDevice::Method& set_waypoint_count_method = modular_server_.createMethod(constants::set_waypoint_count_method_name);
  set_waypoint_count_method.addParameter(waypoint_count_parameter);
  set_waypoint_count_method.attachCallback(callbacks::setWaypointCountCallback);

  ModularDevice::Method& set_waypoint_travel_duration_method = modular_server_.createMethod(constants::set_waypoint_travel_duration_method_name);
  set_waypoint_travel_duration_method.addParameter(waypoint_travel_duration_parameter);
  set_waypoint_travel_duration_method.attachCallback(callbacks::setWaypointTravelDurationCallback);

  ModularDevice::Method& set_waypoint_repeat_method = modular_server_.createMethod(constants::set_waypoint_repeat_method_name);
  set_waypoint_repeat_method.addParameter(waypoint_repeat_parameter);
  set_waypoint_repeat_method.attachCallback(callbacks::setWaypointRepeatCallback);

  ModularDevice::Method& set_waypoint_repeat_period_method = modular_server_.createMethod(constants::set_waypoint_repeat_period_method_name);
  set_waypoint_repeat_period_method.addParameter(waypoint_repeat_period_parameter);
  set_waypoint_repeat_period_method.attachCallback(callbacks::setWaypointRepeatPeriodCallback);

  // Setup Streams
  Serial.begin(constants::baudrate);

  // Start Server
  modular_server_.startServer();
}

void Controller::update()
{
  modular_server_.handleServerRequests();
}

ModularDevice::ModularServer& Controller::getModularServer()
{
  return modular_server_;
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
  modular_server_.getSavedVariableValue(constants::mode_name,mode);
  if (mode == constants::WAYPOINT)
  {
    bool waypoint_repeat;
    modular_server_.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
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
  modular_server_.getSavedVariableValue(constants::mode_name,mode);
  if (mode == constants::WAYPOINT)
  {
    bool waypoint_repeat;
    modular_server_.getSavedVariableValue(constants::waypoint_repeat_parameter_name,waypoint_repeat);
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
  modular_server_.getSavedVariableValue(constants::tone_frequency_parameter_name,tone_frequency);

  int tone_duration;
  modular_server_.getSavedVariableValue(constants::tone_duration_parameter_name,tone_duration);

  tone(constants::speaker_pin,tone_frequency,tone_duration);
}

void Controller::goToNextWaypoint()
{
  bool play_tone_before_move;
  modular_server_.getSavedVariableValue(constants::play_tone_before_move_parameter_name,play_tone_before_move);
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
  modular_server_.setSavedVariableValue(constants::waypoint_count_parameter_name,waypoint_count);
  motor_drive_.setSpeed();
}

void Controller::setWaypointTravelDuration(int waypoint_travel_duration)
{
  motor_drive_.stopAll();
  motor_drive_.zeroAll();
  modular_server_.setSavedVariableValue(constants::waypoint_travel_duration_parameter_name,waypoint_travel_duration);
  motor_drive_.setSpeed();
}

void Controller::startWaypointRepeat()
{
  if (!waypoint_repeating_)
  {
    waypoint_repeating_ = true;
    int waypoint_repeat_period;
    modular_server_.getSavedVariableValue(constants::waypoint_repeat_period_parameter_name,waypoint_repeat_period);
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
