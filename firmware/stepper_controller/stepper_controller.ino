#include "Flash.h"
#include <EEPROM.h>
#include "Streaming.h"
#include "ArduinoJson.h"
#include "JsonSanitizer.h"
#include "JsonStream.h"
#include "Array.h"
#include "Vector.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "SavedVariable.h"
#include "ModularServer.h"

#include "Globals.h"
#include "Constants.h"
#include "Callbacks.h"
#include "Controller.h"

#include "EventController.h"
#include "Stepper.h"
#include "MotorDrive.h"


// See README.md for more information

void setup()
{
  controller.setup();
}

void loop()
{
  controller.update();
}
