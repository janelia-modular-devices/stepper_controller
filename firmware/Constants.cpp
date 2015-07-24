// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace constants
{
const int baudrate = 9600;

const int model_number = 1140;
const int firmware_number = 1;

// HardwareSerial& display_serial = Serial3;

// const int enc_a_pin = 18;
// const int enc_b_pin = 2;
// const int enc_btn_pin = 19;
// const int enc_btn_int = 4;
// const int btn_pin = 21;
// const int btn_int = 2;
// const int led_pwr_pin = 17;
// const int bnc_b_pin = 3;
// const int standalone_update_period = 300;

const int speaker_pin = 46;
const int drive_enable_pin = 49;
const int step_pins[MOTOR_COUNT] = {53};
const int dir_pins[MOTOR_COUNT] = {51};
const int waypoint_interrupt_pin = 2;
const int tone_interrupt_pin = 3;

const ModeType mode_default = WAYPOINT;

const uint8_t enable_polarity_default = HIGH;

const char orientation_inverted = '-';

const int steps_per_rev = 200;
const int micro_steps_per_step_default = 256;
const int micro_steps_per_step_min = 1;
const int micro_steps_per_step_max = 256;

const int waypoint_count_default = 16;
const int waypoint_count_min = 1;
const int waypoint_count_max = 255;
const int waypoint_travel_time_default = 400;
const int waypoint_travel_time_min = 1;
const int waypoint_travel_time_max = 8000;
const bool waypoint_repeat_default = false;
const int waypoint_repeat_period_default = 1000;
const int waypoint_repeat_period_min = 1;
const int waypoint_repeat_period_max = 8000;

const int tone_frequency_min = 850;
const int tone_frequency_max = 20000;
const int tone_duration_min = 1;
const int tone_duration_max = 10000;
const bool play_tone_before_waypoint_move_default = true;

const int waypoint_interrupt_number = 0;
const int waypoint_interrupt_mode_default = FALLING;
const int tone_interrupt_number = 1;
const int tone_interrupt_mode_default = FALLING;
const int tone_interrupt_frequency_default = 5000;
const int tone_interrupt_duration_default = 250;

CONSTANT_STRING(device_name,"stepper_controller");

CONSTANT_STRING(mfc_parameter_name,"mfc");

// CONSTANT_STRING(execute_standalone_callback_method_name,"executeStandaloneCallback");
// CONSTANT_STRING(get_leds_powered_method_name,"getLedsPowered");
CONSTANT_STRING(set_mfc_flow_method_name,"setMfcFlow");

// CONSTANT_STRING(frame0_name,"setMfcFlows %");
// CONSTANT_STRING(frame1_name,"getMfcFlowMeasures %");
// CONSTANT_STRING(frame2_name,"getAnalogInputs %");
// const ConstantString frame_name_array[] =
//   {
//     frame0_name,
//     frame1_name,
//     frame2_name,
//   };
}
