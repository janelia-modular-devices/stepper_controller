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

  // Pin Setup
  for (int mfc = 0; mfc < constants::MFC_COUNT; mfc++)
  {
    pinMode(constants::mfc_purge_pins[mfc],INPUT);
    pinMode(constants::mfc_valve_off_pins[mfc],INPUT);
  }
  pinMode(constants::bnc_b_pin,OUTPUT);

  // Device Info
  modular_device.setName(constants::device_name);
  modular_device.setModelNumber(constants::model_number);
  modular_device.setFirmwareNumber(constants::firmware_number);

  // Saved Variables
  modular_device.createSavedVariable(constants::states_name,
                                     constants::states_array_default,
                                     constants::STATE_COUNT);

  int default_state = 0;
  modular_device.getSavedVariableValue(constants::states_name,flow_settings_array_,default_state);
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    setMfcFlow(mfc,flow_settings_array_[mfc]);
  }

  // Parameters
  ModularDevice::Parameter& mfc_parameter = modular_device.createParameter(constants::mfc_parameter_name);
  mfc_parameter.setRange(0,constants::MFC_COUNT-1);

  ModularDevice::Parameter& ain_parameter = modular_device.createParameter(constants::ain_parameter_name);
  ain_parameter.setRange(0,constants::AIN_COUNT-1);

  ModularDevice::Parameter& percent_parameter = modular_device.createParameter(constants::percent_parameter_name);
  percent_parameter.setRange(constants::percent_min,constants::percent_max);

  ModularDevice::Parameter& percents_parameter = modular_device.createParameter(constants::percents_parameter_name);
  percents_parameter.setTypeArray();
  percents_parameter.setRange(constants::percent_min,constants::percent_max);

  ModularDevice::Parameter& state_parameter = modular_device.createParameter(constants::state_parameter_name);
  state_parameter.setRange(0,constants::STATE_COUNT-1);

  ModularDevice::Parameter& duration_parameter = modular_device.createParameter(constants::duration_parameter_name);
  duration_parameter.setRange(constants::duration_min,constants::duration_max);
  duration_parameter.setUnits(constants::duration_parameter_units);

  // Methods
  ModularDevice::Method& execute_standalone_callback_method = modular_device.createMethod(constants::execute_standalone_callback_method_name);
  execute_standalone_callback_method.attachCallback(callbacks::executeStandaloneCallbackCallback);

  ModularDevice::Method& get_leds_powered_method = modular_device.createMethod(constants::get_leds_powered_method_name);
  get_leds_powered_method.attachCallback(callbacks::getLedsPoweredCallback);

  ModularDevice::Method& set_mfc_flow_method = modular_device.createMethod(constants::set_mfc_flow_method_name);
  set_mfc_flow_method.attachCallback(callbacks::setMfcFlowCallback);
  set_mfc_flow_method.addParameter(mfc_parameter);
  set_mfc_flow_method.addParameter(percent_parameter);

  ModularDevice::Method& set_mfc_flows_method = modular_device.createMethod(constants::set_mfc_flows_method_name);
  set_mfc_flows_method.attachCallback(callbacks::setMfcFlowsCallback);
  set_mfc_flows_method.addParameter(percents_parameter);

  ModularDevice::Method& get_mfc_flow_setting_method = modular_device.createMethod(constants::get_mfc_flow_setting_method_name);
  get_mfc_flow_setting_method.attachCallback(callbacks::getMfcFlowSettingCallback);
  get_mfc_flow_setting_method.addParameter(mfc_parameter);

  ModularDevice::Method& get_mfc_flow_settings_method = modular_device.createMethod(constants::get_mfc_flow_settings_method_name);
  get_mfc_flow_settings_method.attachCallback(callbacks::getMfcFlowSettingsCallback);

  ModularDevice::Method& get_mfc_flow_measure_method = modular_device.createMethod(constants::get_mfc_flow_measure_method_name);
  get_mfc_flow_measure_method.attachCallback(callbacks::getMfcFlowMeasureCallback);
  get_mfc_flow_measure_method.addParameter(mfc_parameter);

  ModularDevice::Method& get_mfc_flow_measures_method = modular_device.createMethod(constants::get_mfc_flow_measures_method_name);
  get_mfc_flow_measures_method.attachCallback(callbacks::getMfcFlowMeasuresCallback);

  ModularDevice::Method& get_analog_input_method = modular_device.createMethod(constants::get_analog_input_method_name);
  get_analog_input_method.attachCallback(callbacks::getAnalogInputCallback);
  get_analog_input_method.addParameter(ain_parameter);

  ModularDevice::Method& get_analog_inputs_method = modular_device.createMethod(constants::get_analog_inputs_method_name);
  get_analog_inputs_method.attachCallback(callbacks::getAnalogInputsCallback);

  ModularDevice::Method& save_state_method = modular_device.createMethod(constants::save_state_method_name);
  save_state_method.attachCallback(callbacks::saveStateCallback);
  save_state_method.addParameter(state_parameter);

  ModularDevice::Method& recall_state_method = modular_device.createMethod(constants::recall_state_method_name);
  recall_state_method.attachCallback(callbacks::recallStateCallback);
  recall_state_method.addParameter(state_parameter);

  ModularDevice::Method& get_saved_states_method = modular_device.createMethod(constants::get_saved_states_method_name);
  get_saved_states_method.attachCallback(callbacks::getSavedStatesCallback);

  ModularDevice::Method& pulse_bnc_b_method = modular_device.createMethod(constants::pulse_bnc_b_method_name);
  pulse_bnc_b_method.addParameter(duration_parameter);
  pulse_bnc_b_method.attachCallback(callbacks::pulseBncBCallback);

  // Start Server
  modular_device.startServer(constants::baudrate);

  // Standalone Interface
  standalone_interface_.setup(constants::frame_name_array,constants::FRAME_COUNT);

  // Display Labels
  Standalone::DisplayLabel* mfc_dsp_lbl_ptr_array[constants::MFC_COUNT];
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    mfc_dsp_lbl_ptr_array[mfc] = &(standalone_interface_.createDisplayLabel());
    mfc_dsp_lbl_ptr_array[mfc]->setDisplayPosition(constants::mfc_dsp_lbl_display_positions[mfc]);
    mfc_dsp_lbl_ptr_array[mfc]->setConstantString(constants::mfc_dsp_lbl_strings[mfc]);
    mfc_dsp_lbl_ptr_array[mfc]->setRightJustify();
  }

  Standalone::DisplayLabel* ain_dsp_lbl_ptr_array[constants::AIN_COUNT];
  for (int ain=0; ain<constants::AIN_COUNT; ain++)
  {
    ain_dsp_lbl_ptr_array[ain] = &(standalone_interface_.createDisplayLabel());
    ain_dsp_lbl_ptr_array[ain]->setDisplayPosition(constants::ain_dsp_lbl_display_positions[ain]);
    ain_dsp_lbl_ptr_array[ain]->setConstantString(constants::ain_dsp_lbl_strings[ain]);
    ain_dsp_lbl_ptr_array[ain]->setRightJustify();
  }

  Standalone::DisplayLabel& state_dsp_lbl = standalone_interface_.createDisplayLabel();
  state_dsp_lbl.setDisplayPosition(constants::state_dsp_lbl_display_position);
  state_dsp_lbl.setConstantString(constants::state_parameter_name);

  Standalone::DisplayLabel& duration_dsp_lbl = standalone_interface_.createDisplayLabel();
  duration_dsp_lbl.setDisplayPosition(constants::duration_dsp_lbl_display_position);
  duration_dsp_lbl.setConstantString(constants::duration_parameter_name);
  duration_dsp_lbl.setRightJustify();

  // Display Variables
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    flow_m_dsp_var_ptr_array_[mfc] = &(standalone_interface_.createDisplayVariable());
    flow_m_dsp_var_ptr_array_[mfc]->setDisplayPosition(constants::flow_var_display_positions[mfc]);
    flow_m_dsp_var_ptr_array_[mfc]->setDisplayWidth(constants::percent_display_width);

    flow_s_dsp_var_ptr_array_[mfc] = &(standalone_interface_.createDisplayVariable());
    flow_s_dsp_var_ptr_array_[mfc]->setDisplayPosition(constants::flow_var_display_positions[mfc]);
    flow_s_dsp_var_ptr_array_[mfc]->setDisplayWidth(constants::percent_display_width);
  }

  for (int ain=0; ain<constants::AIN_COUNT; ain++)
  {
    ain_dsp_var_ptr_array_[ain] = &(standalone_interface_.createDisplayVariable());
    ain_dsp_var_ptr_array_[ain]->setDisplayPosition(constants::ain_dsp_var_display_positions[ain]);
    ain_dsp_var_ptr_array_[ain]->setDisplayWidth(constants::percent_display_width);
  }

  // Interactive Variables
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    flow_int_var_ptr_array_[mfc] = &(standalone_interface_.createInteractiveVariable());
    flow_int_var_ptr_array_[mfc]->setDisplayPosition(constants::flow_var_display_positions[mfc]);
    flow_int_var_ptr_array_[mfc]->setDisplayWidth(constants::percent_display_width);
    flow_int_var_ptr_array_[mfc]->setRange(constants::percent_min,constants::percent_max);
    flow_int_var_ptr_array_[mfc]->setValue(flow_settings_array_[mfc]);
  }

  state_int_var_ptr_ = &(standalone_interface_.createInteractiveVariable());
  state_int_var_ptr_->setDisplayPosition(constants::state_int_var_display_position);
  state_int_var_ptr_->setRange(0,constants::STATE_COUNT-1);
  state_int_var_ptr_->setDisplayWidth(2);

  duration_int_var_ptr_ = &(standalone_interface_.createInteractiveVariable());
  duration_int_var_ptr_->setDisplayPosition(constants::duration_int_var_display_position);
  duration_int_var_ptr_->setRange(constants::duration_min,constants::duration_max);
  duration_int_var_ptr_->trimDisplayWidthUsingRange();
  duration_int_var_ptr_->setValue(constants::duration_int_var_default);

  // All Frames

  // Frame 0
  int frame = 0;
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    mfc_dsp_lbl_ptr_array[mfc]->addToFrame(frame);
    flow_int_var_ptr_array_[mfc]->addToFrame(frame);
  }

  // Frame 1
  frame = 1;
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    mfc_dsp_lbl_ptr_array[mfc]->addToFrame(frame);
    flow_m_dsp_var_ptr_array_[mfc]->addToFrame(frame);
  }

  // Frame 2
  frame = 2;
  for (int ain=0; ain<constants::AIN_COUNT; ain++)
  {
    ain_dsp_lbl_ptr_array[ain]->addToFrame(frame);
    ain_dsp_var_ptr_array_[ain]->addToFrame(frame);
  }

  // Frame 3
  frame = 3;
  state_dsp_lbl.addToFrame(frame);
  state_int_var_ptr_->addToFrame(frame);
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    mfc_dsp_lbl_ptr_array[mfc]->addToFrame(frame);
    flow_int_var_ptr_array_[mfc]->addToFrame(frame);
  }
  standalone_interface_.attachCallbackToFrame(callbacks::saveStateStandaloneCallback,frame);

  // Frame 4
  frame = 4;
  state_dsp_lbl.addToFrame(frame);
  state_int_var_ptr_->addToFrame(frame);
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    mfc_dsp_lbl_ptr_array[mfc]->addToFrame(frame);
    flow_s_dsp_var_ptr_array_[mfc]->addToFrame(frame);
  }
  standalone_interface_.attachCallbackToFrame(callbacks::recallStateStandaloneCallback,frame);

  // Frame 5
  frame = 5;
  duration_dsp_lbl.addToFrame(frame);
  duration_int_var_ptr_->addToFrame(frame);
  standalone_interface_.attachCallbackToFrame(callbacks::pulseBncBStandaloneCallback,frame);

  // Enable Standalone Interface
  standalone_interface_.enable();
}

void Controller::update()
{
  modular_device.handleServerRequests();
  boolean updated = standalone_interface_.update();
  if (updated)
  {
    updateDisplayVariables();
    updateMfcFlow();
  }
}

void Controller::executeStandaloneCallback()
{
  standalone_interface_.executeCurrentFrameCallback();
}

bool Controller::getLedsPowered()
{
  return digitalRead(constants::led_pwr_pin) == HIGH;
}

void Controller::setMfcFlow(const uint8_t mfc, uint8_t percent)
{
  if (mfc >= constants::MFC_COUNT)
  {
    return;
  }
  percent = constrain(percent,constants::percent_min,constants::percent_max);
  if (percent != flow_settings_array_[mfc])
  {
    flow_settings_array_[mfc] = percent;
    flow_int_var_ptr_array_[mfc]->setValue(percent);
    int pwm_value = betterMap(percent,
                              constants::percent_min,
                              constants::percent_max,
                              constants::pwm_min,
                              constants::pwm_max);
    analogWrite(constants::mfc_pwm_pins[mfc],pwm_value);
  }
}

uint8_t Controller::getMfcFlowSetting(const uint8_t mfc)
{
  if (mfc >= constants::MFC_COUNT)
  {
    return 0;
  }
  return flow_settings_array_[mfc];
}

uint8_t Controller::getMfcFlowMeasure(const uint8_t mfc)
{
  if (mfc >= constants::MFC_COUNT)
  {
    return 0;
  }
  int ain_value = analogRead(constants::mfc_ain_pins[mfc]);
  int percent = betterMap(ain_value,
                          constants::ain_min,
                          constants::ain_max,
                          constants::percent_min,
                          constants::percent_max);
  return percent;
}

uint8_t Controller::getAnalogInput(const uint8_t ain)
{
  if (ain >= constants::AIN_COUNT)
  {
    return 0;
  }
  int ain_value = analogRead(constants::ain_pins[ain]);
  int percent = betterMap(ain_value,
                          constants::ain_min,
                          constants::ain_max,
                          constants::percent_min,
                          constants::percent_max);
  return percent;
}

void Controller::updateDisplayVariables()
{
  int percent;
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    percent = getMfcFlowMeasure(mfc);
    flow_m_dsp_var_ptr_array_[mfc]->setValue(percent);

    percent = getMfcFlowSetting(mfc);
    flow_s_dsp_var_ptr_array_[mfc]->setValue(percent);
  }

  for (int ain=0; ain<constants::AIN_COUNT; ain++)
  {
    percent = getAnalogInput(ain);
    ain_dsp_var_ptr_array_[ain]->setValue(percent);
  }
}

void Controller::updateMfcFlow()
{
  int percent;
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    percent = flow_int_var_ptr_array_[mfc]->getValue();
    setMfcFlow(mfc,percent);
  }
}

void Controller::saveState(int state)
{
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    states_array_[state][mfc] = flow_settings_array_[mfc];
  }
  modular_device.setSavedVariableValue(constants::states_name,states_array_,state);
}

void Controller::recallState(int state)
{
  modular_device.getSavedVariableValue(constants::states_name,states_array_,state);
  for (int mfc=0; mfc<constants::MFC_COUNT; mfc++)
  {
    setMfcFlow(mfc,states_array_[state][mfc]);
  }
}

void Controller::getStatesArray(uint8_t states_array[][constants::MFC_COUNT])
{
  for (int state=0; state<constants::STATE_COUNT; state++)
  {
    modular_device.getSavedVariableValue(constants::states_name,states_array,state);
  }
}

uint8_t Controller::getStateIntVar()
{
  return state_int_var_ptr_->getValue();
}

uint8_t Controller::getDurationIntVar()
{
  return duration_int_var_ptr_->getValue();
}

Controller controller;
