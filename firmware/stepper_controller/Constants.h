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
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{SAVED_VARIABLE_COUNT_MAX=10};
enum{PARAMETER_COUNT_MAX=15};
enum{METHOD_COUNT_MAX=30};

enum ModeType {WAYPOINT};
enum {MOTOR_COUNT=1};

extern const int baudrate;

extern const int model_number;

extern const unsigned char firmware_major;
extern const unsigned char firmware_minor;
extern const unsigned char firmware_patch;

extern const int speaker_pin;
extern const int drive_enable_pin;
extern const int step_pins[MOTOR_COUNT];
extern const int dir_pins[MOTOR_COUNT];
extern const int motion_switch_interrupt_pin;
extern const int sound_switch_interrupt_pin;
extern const int motion_bnc_interrupt_pin;
extern const int sound_bnc_interrupt_pin;

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
extern const int tone_frequency_default;
extern const int tone_duration_min;
extern const int tone_duration_max;
extern const int tone_duration_default;
extern const bool play_tone_before_move_default;

extern const int motion_switch_interrupt_number;
extern const int motion_switch_interrupt_mode_default;
extern const int sound_switch_interrupt_number;
extern const int sound_switch_interrupt_mode_default;

extern const int motion_bnc_interrupt_number;
extern const int motion_bnc_interrupt_mode_default;
extern const int sound_bnc_interrupt_number;
extern const int sound_bnc_interrupt_mode_default;

extern ConstantString device_name;

extern ConstantString duration_parameter_units;
extern ConstantString frequency_parameter_units;

extern ConstantString micro_steps_per_step_parameter_name;
extern ConstantString tone_frequency_parameter_name;
extern ConstantString tone_duration_parameter_name;
extern ConstantString play_tone_before_move_parameter_name;
extern ConstantString waypoint_count_parameter_name;
extern ConstantString waypoint_travel_duration_parameter_name;
extern ConstantString waypoint_repeat_parameter_name;
extern ConstantString waypoint_repeat_period_parameter_name;

extern ConstantString enable_method_name;
extern ConstantString disable_method_name;
extern ConstantString stop_method_name;
extern ConstantString get_controller_info_method_name;
extern ConstantString move_method_name;
extern ConstantString play_tone_method_name;
extern ConstantString set_tone_frequency_method_name;
extern ConstantString set_tone_duration_method_name;
extern ConstantString set_play_tone_before_move_method_name;
extern ConstantString set_waypoint_count_method_name;
extern ConstantString set_waypoint_travel_duration_method_name;
extern ConstantString set_waypoint_repeat_method_name;
extern ConstantString set_waypoint_repeat_period_method_name;

extern ConstantString mode_name;
extern ConstantString enable_polarity_name;

extern ConstantString waypoint_parameter_error;
}
#endif
