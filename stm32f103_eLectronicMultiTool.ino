String projName = F("LMTool v0.1.1");
void empty_loop();

#define NUM_MENU_PAGES 9
String strMenuPages[] = {
  "Tester", "PWM generator", "Servo tester",
  "Piezo tester", "Sensors", "IR Rx / Tx",
  "I2C Scanner", "Continuity", "ATTiny programmer"
};
int8_t selMenu = 0;

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


#include <filter.h>
#define LEN_FIL_R 5
filter fiR( LEN_FIL_R );

#include <DHT.h>


// dependecies
#include "imgs.h"     // Graphics
#include "pins.h"     // PINS
#include "sys_funs.h" // system functions
#include "btns.h"     // Buttons
#include "encoder.h"  // Encoder


// +--------------------------+
// | Sensors:                 |
// |   DHT11;   (tmp & humid) |
// |   HC-SR04; (distance)    |
// |   MPU6050; (gyro)        |
// |   VS1838B; (IR)          |
// +--------------------------+
#include "sensrs.h"

//#include "multimeter.h"

//#include "dst_gyro.h"
#include "components_tests.h"


#include "loop_main_screen.h"
#include "loop_continuity.h"
#include "loop_IR.h"
#include "loop_PWM_gen.h"
#include "loop_servo.h"
#include "loop_piezo.h"
#include "loop_sensors.h"
#include "loop_I2C_scanner.h"

// must be after all deps
#include "bootup.h"   // bootup function


void empty_loop() {
  onEnterSubLoop();

  display.setTextColor(1, 0);
  display.setTextSize(1);
  display.setCursor(55, 24);
  display.print("Not");
  display.setCursor(31, 32);
  display.print("Implemented");
  display.display();

  while ( !exitCode ) {
    exitCodeSub = false;
    conDelay( 1000, exitCode || exitCodeSub );
  }

  onExitSubLoop();
}

void (*menus[NUM_MENU_PAGES])() {
  loop_tester,        // 1 Tester
  loop_PWM_gen,       // 2 PWM generator
  loop_servo,         // 3 Servo tester
  loop_piezo,         // 4 Piezo tester
  loop_sensors,       // 5 Sensors
  loop_IR,            // 6 IR Rx / Tx
  loop_I2C_scanner,   // 7 I2C scanner
  loop_continuity,    // 8 Continuity
  empty_loop          // 9 ATTiny programmer
};



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
  menus[selMenu]();
}
