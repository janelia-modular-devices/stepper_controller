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
  motor_drive_.setup();
  Timer1.initialize();
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

  // Parameters
  // ModularDevice::Parameter& mfc_parameter = modular_device.createParameter(constants::mfc_parameter_name);
  // mfc_parameter.setRange(0,constants::MFC_COUNT-1);

  // Methods
  // ModularDevice::Method& execute_standalone_callback_method = modular_device.createMethod(constants::execute_standalone_callback_method_name);
  // execute_standalone_callback_method.attachCallback(callbacks::executeStandaloneCallbackCallback);

  // ModularDevice::Method& get_leds_powered_method = modular_device.createMethod(constants::get_leds_powered_method_name);
  // get_leds_powered_method.attachCallback(callbacks::getLedsPoweredCallback);

  // ModularDevice::Method& set_mfc_flow_method = modular_device.createMethod(constants::set_mfc_flow_method_name);
  // set_mfc_flow_method.attachCallback(callbacks::setMfcFlowCallback);
  // set_mfc_flow_method.addParameter(mfc_parameter);
  // set_mfc_flow_method.addParameter(percent_parameter);

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

void Controller::update()
{
  modular_device.handleServerRequests();
  // boolean updated = standalone_interface_.update();
  // if (updated)
  // {
  //   updateDisplayVariables();
  // }
 }

// void Controller::executeStandaloneCallback()
// {
//   standalone_interface_.executeCurrentFrameCallback();
// }

// bool Controller::getLedsPowered()
// {
//   return digitalRead(constants::led_pwr_pin) == HIGH;
// }

void SystemState::attachWaypointInterrupt()
{
  pinMode(constants::waypoint_interrupt_pin, INPUT);
  digitalWrite(constants::waypoint_interrupt_pin, HIGH);
  attachInterrupt(constants::waypoint_interrupt_number,waypointISR,savedVariables.getWaypointInterruptMode());
}

void SystemState::attachToneInterrupt() {
  pinMode(constants::toneInterruptPin, INPUT);
  digitalWrite(constants::toneInterruptPin, HIGH);
  attachInterrupt(constants::toneInterruptNumber,toneISR,savedVariables.getToneInterruptMode());
}

Controller controller;


