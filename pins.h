// +------+
// | PINS |
// +------+

/*
 .  5 V tolerant
 o  not 5 V tolerant
 -  normal pin
 ~  PWM pin
 X  restricted/errors

left side:
 PB12 .- ENC_A   // HC-SR04 TRIG
 PB13 .-    // HC-SR04 ECHO
 PB14 .- ENC_B
 PB15 .- ENC_BTN
 PA8  .~    // DHT11
 PA9  .~ TX
 PA10 .~ RX
 PA11 .- 
 PA12 .- 
 PA15 .- IR RECV
 PB3  .- BTN A
 PB4  .- BTN B
 PB5  o- 
 PB6  .~ X
 PB7  .~ X
 PB8  .~ PWM OUT
 PB9  .~ IR SEND


right side:
 PB11 .- SDA2
 PB10 .- SCL2
 PB1  o~          // light
 PB0  o~ PIN 3 R2
 PA7  o~ PIN 3
 PA6  o~ PIN 3 R1
 PA5  o- PIN 2 R2
 PA4  o- PIN 2
 PA3  o~ PIN 2 R1
 PA2  o~ PIN 1 R2
 PA1  o~ PIN 1
 PA0  o~ PIN 1 R1
 PC15 o! 
 PC14 o! 
 PC13 o! onboard LED
*/

/*
include:
 PIN_V_IN
 PIN_A_IN
 PIN_COM
*/

#define PIN_ENC_A   PB12
#define PIN_ENC_B   PB14
#define PIN_ENC_BTN PB15


#define PIN_BTN_A PB3
#define PIN_BTN_B PB4
//#define PIN_BTN_C PB5

#define PIN_DHT11 PA8

#define PIN_1 PA1
#define PIN_2 PA4
#define PIN_3 PA7

#define PIN_1_R1 PA0 // 470 Ohm
#define PIN_1_R2 PA2 // 2 MOhm
#define PIN_2_R1 PA3 // 470 Ohm
#define PIN_2_R2 PA5 // 2 MOhm
#define PIN_3_R1 PA6 // 470 Ohm
#define PIN_3_R2 PB0 // 2 MOhm



#define PIN_LIGHT_SENS PB1

#define PIN_DST_TRIG PB12 // PB4
#define PIN_DST_ECHO PB13 // PB5

#define PIN_PWM_OUT PB8

#define PIN_IR_SEND PB9
#define PIN_IR_RECV PA15

#define NUM_PIN_SUPPROT 6
uint32_t pinsSupportR[] = { 475, 100650, 457, 100150, 466, 101080 }; // 2000000
uint32_t pinsSupport[] = { PIN_1_R1, PIN_1_R2, PIN_2_R1, PIN_2_R2, PIN_3_R1, PIN_3_R2 };
uint32_t pinsSupportLowR[] = { PIN_1_R1, PIN_2_R1, PIN_3_R1 };
uint32_t pins123[] = { PIN_1, PIN_2, PIN_3 };
uint32_t pinA, pinB;
void pinOff( uint32_t ui4Pin ) { pinMode( ui4Pin, INPUT ); }
void clearPins();
