String projName = F("LMTool v0.1.1");
void empty_loop();

#include <IRremote.h>
#include <Wire.h>
#include <SoftWire.h>
#include <Servo.h>
Servo myservo;

TwoWire WIRE2 (2, I2C_FAST_MODE); // use IIC2
#define Wire WIRE2


#include <OLED_I2C_light.h>
OLED  display( &Wire );
extern uint8_t SmallFont[];


// simple menus
#include "simpleMenu.h"


#include <filter.h>
#define LEN_FIL_R 5
filter fiR( LEN_FIL_R );


// dependecies
#include "imgs.h"     // Graphics
#include "pins.h"     // PINS
#include "sys_funs.h" // system functions
#include "btns.h"     // Buttons
#include "encoder.h"  // Encoder

// loops
#include "components_tests.h"
#include "loop_continuity.h"
#include "loop_IR.h"
#include "loop_PWM_gen.h"
#include "loop_servo.h"
#include "loop_piezo.h"
#include "loop_sensors.h"
#include "loop_I2C_scanner.h"

#include "loop_pinout.h"

// must be after all deps
#include "bootup.h"   // bootup function



#define NUM_MENU_PAGES 10
String strMenuPages[] = {
  "Tester", "PWM generator", "Servo tester",
  "Piezo tester", "Sensors", "IR Rx / Tx",
  "I2C Scanner", "Continuity", "ATTiny programmer",
  "Pinout"
};
Menu mainMenu( strMenuPages, NUM_MENU_PAGES );

void (*menus[NUM_MENU_PAGES])() {
  loop_tester,        // 1 Tester
  loop_PWM_gen,       // 2 PWM generator
  loop_servo,         // 3 Servo tester
  loop_piezo,         // 4 Piezo tester
  loop_sensors,       // 5 Sensors
  loop_IR,            // 6 IR Rx / Tx
  loop_I2C_scanner,   // 7 I2C scanner
  loop_continuity,    // 8 Continuity
  empty_loop,         // 9 ATTiny programmer
  loop_pinout
};

#include "loop_main_screen.h"


// +----------------+
// | Main functions |
// +----------------+

void setup() {
  Serial.begin(9600);

  Wire.begin();
  display.begin();
  bootup();

  delay( 300 );
  display.clearDisplay();
  display.display();
}



void loop() {
  //  checkBattery();
  loop_mainScreen(); // temp, humidity and light info
  menus[mainMenu.id]();
}
