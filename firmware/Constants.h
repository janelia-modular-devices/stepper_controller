// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "ConstantVariable.h"

namespace constants
{
enum ModeType {WAYPOINT};
enum {MOTOR_COUNT=1};

extern const int baudrate;

extern const int model_number;
extern const int firmware_number;

// extern HardwareSerial& display_serial;

// extern const int enc_a_pin;
// extern const int enc_b_pin;
// extern const int enc_btn_pin;
// extern const int enc_btn_int;
// extern const int btn_pin;
// extern const int btn_int;
// extern const int led_pwr_pin;
// extern const int bnc_b_pin;
// extern const int standalone_update_period;

extern const int speaker_pin;
extern const int drive_enable_pin;
extern const int step_pins[MOTOR_COUNT];
extern const int dir_pins[MOTOR_COUNT];
extern const int waypoint_interrupt_pin;
extern const int tone_interrupt_pin;

extern const ModeType mode_default;

extern const uint8_t enable_polarity_default;

extern const char orientation_inverted;

extern const int steps_per_rev;
extern const int micro_steps_per_step_default;
extern const int micro_steps_per_step_min;
extern const int micro_steps_per_step_max;

extern const int waypoint_count_default;
extern const int waypoint_count_min;
extern const int waypoint_count_max;
extern const int waypoint_travel_duration_default;
extern const int waypoint_travel_duration_min;
extern const int waypoint_travel_duration_max;
extern const bool waypoint_repeat_default;
extern const int waypoint_repeat_period_default;
extern const int waypoint_repeat_period_min;
extern const int waypoint_repeat_period_max;

extern const int tone_frequency_min;
extern const int tone_frequency_max;
extern const int tone_duration_min;
extern const int tone_duration_max;
extern const bool play_tone_before_waypoint_move_default;

extern const int waypoint_interrupt_number;
extern const int waypoint_interrupt_mode_default;
extern const int tone_interrupt_number;
extern const int tone_interrupt_mode_default;
extern const int tone_interrupt_frequency_default;
extern const int tone_interrupt_duration_default;

extern ConstantString device_name;

extern ConstantString waypoint_count_parameter_name;
extern ConstantString micro_steps_per_step_parameter_name;
extern ConstantString waypoint_travel_duration_parameter_name;

// extern ConstantString execute_standalone_callback_method_name;
// extern ConstantString get_leds_powered_method_name;
extern ConstantString set_mfc_flow_method_name;

// extern ConstantString frame0_name;
// extern ConstantString frame1_name;
// extern ConstantString frame2_name;
// extern const ConstantString frame_name_array[FRAME_COUNT];
extern ConstantString mode_name;
extern ConstantString enable_polarity_name;
}
#endif
