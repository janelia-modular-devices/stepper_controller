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

// Use semantic versioning http://semver.org/
const unsigned char firmware_major = 0;
const unsigned char firmware_minor = 1;
const unsigned char firmware_patch = 0;

const int speaker_pin = 46;
const int drive_enable_pin = 49;
const int step_pins[MOTOR_COUNT] = {53};
const int dir_pins[MOTOR_COUNT] = {51};
const int motion_switch_interrupt_pin = 2;
const int sound_switch_interrupt_pin = 3;
const int motion_bnc_interrupt_pin = 21;
const int sound_bnc_interrupt_pin = 20;

const ModeType mode_default = WAYPOINT;

const uint8_t enable_polarity_default = HIGH;

const char orientation_inverted = '-';

const int steps_per_rev = 200;
const int micro_steps_per_step_default = 256;
const int micro_steps_per_step_min = 1;
const int micro_steps_per_step_max = 256;

const int waypoint_count_default = 30;
const int waypoint_count_min = 1;
const int waypoint_count_max = 255;
const int waypoint_travel_duration_default = 400;
const int waypoint_travel_duration_min = 100;
const int waypoint_travel_duration_max = 10000;
const bool waypoint_repeat_default = false;
const int waypoint_repeat_period_default = 1000;
const int waypoint_repeat_period_min = 100;
const int waypoint_repeat_period_max = 10000;

const int tone_frequency_min = 850;
const int tone_frequency_max = 20000;
const int tone_frequency_default = 5000;
const int tone_duration_min = 1;
const int tone_duration_max = 10000;
const int tone_duration_default = 250;
const bool play_tone_before_move_default = false;

const int motion_switch_interrupt_number = 0;
const int motion_switch_interrupt_mode_default = FALLING;
const int sound_switch_interrupt_number = 1;
const int sound_switch_interrupt_mode_default = FALLING;

const int motion_bnc_interrupt_number = 2;
const int motion_bnc_interrupt_mode_default = RISING;
const int sound_bnc_interrupt_number = 3;
const int sound_bnc_interrupt_mode_default = RISING;

CONSTANT_STRING(device_name,"stepper_controller");

CONSTANT_STRING(duration_parameter_units,"ms");
CONSTANT_STRING(frequency_parameter_units,"Hz");

CONSTANT_STRING(micro_steps_per_step_parameter_name,"micro_steps_per_step_count");
CONSTANT_STRING(tone_frequency_parameter_name,"tone_frequency");
CONSTANT_STRING(tone_duration_parameter_name,"tone_duration");
CONSTANT_STRING(play_tone_before_move_parameter_name,"play_tone_before_move");
CONSTANT_STRING(waypoint_count_parameter_name,"waypoint_count");
CONSTANT_STRING(waypoint_travel_duration_parameter_name,"waypoint_travel_duration");
CONSTANT_STRING(waypoint_repeat_parameter_name,"waypoint_repeat");
CONSTANT_STRING(waypoint_repeat_period_parameter_name,"waypoint_repeat_period");

CONSTANT_STRING(enable_method_name,"enable");
CONSTANT_STRING(disable_method_name,"disable");
CONSTANT_STRING(stop_method_name,"stop");
CONSTANT_STRING(get_controller_info_method_name,"getControllerInfo");
CONSTANT_STRING(move_method_name,"move");
CONSTANT_STRING(play_tone_method_name,"playTone");
CONSTANT_STRING(set_tone_frequency_method_name,"setToneFrequency");
CONSTANT_STRING(set_tone_duration_method_name,"setToneDuration");
CONSTANT_STRING(set_play_tone_before_move_method_name,"setPlayToneBeforeMove");
CONSTANT_STRING(set_waypoint_count_method_name,"setWaypointCount");
CONSTANT_STRING(set_waypoint_travel_duration_method_name,"setWaypointTravelDuration");
CONSTANT_STRING(set_waypoint_repeat_method_name,"setWaypointRepeat");
CONSTANT_STRING(set_waypoint_repeat_period_method_name,"setWaypointRepeatPeriod");

CONSTANT_STRING(mode_name,"mode");
CONSTANT_STRING(enable_polarity_name,"enable_polarity");

CONSTANT_STRING(waypoint_parameter_error,"waypoint_travel_duration cannot be greater than waypoint_repeat_period when waypoint_repeat is true.");
}
