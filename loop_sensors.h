// +-----------------------+
// | Sensor tester Section |
// +-----------------------+

#include "libs\arduino-DHT_light\DHT_light.h"
DHT_light dht;
const String cstrFail = "FAIL";
const String cstrCheck = "?   ";

//  T1    T2    T3    PWM_OUT
//  1     2     3     4
//  VCC   DATA  NC    GND


#define NUM_DHT_PAGES 5
const DHT_light::DHT_MODEL_t dhtTypes[] = {
  DHT_light::AUTO_DETECT, DHT_light::DHT11,
  DHT_light::DHT22, DHT_light::AM2302,  /* Packaged DHT22*/ DHT_light::RHT03 };
String strSensorsDHT[] = { "Auto", "DHT11", "DHT22", "AM2302",  /* Packaged DHT22*/ "RHT03" };
Menu menuSensorsDHT( strSensorsDHT, NUM_DHT_PAGES );

void loop_DHT() {
  onEnterSubLoop();
  float humid = 0, temp = 0, humidOld = 0, tempOld = 0;
  bool requireUpdate = true;
  unsigned long ulLastDHT = 0, ulDHTWaitFor = 0, ulDHTWaitTo = 0;
  
  display.setTextSize( 1 );
  display.setCursor( 0, 0 ); display.print( "Sensors DHT" );
  display.setCursor(0, 12); display.print("Type:");
  display.setCursor(38, 12); display.print("Status:");
  
  display.setTextSize(2);
  display.setCursor(115, 25); display.print("C");
  display.setCursor(115, 44); display.print("%");

  display.drawBitmap(49, 24, BMP_TEMP, 8, 16, 1);
  display.drawBitmap(47, 43, BMP_HUMID, 12, 16, 1);
  display.display();
  
  pinMode( PIN_1, OUTPUT ); pinMode( PIN_2, OUTPUT );
  pinMode( PIN_3, OUTPUT ); pinMode( PIN_PWM_OUT, OUTPUT );
  digitalWrite( PIN_1, HIGH );  digitalWrite( PIN_2, HIGH );
  digitalWrite( PIN_3, LOW );   digitalWrite( PIN_PWM_OUT, LOW );
  
  do {
    display.fillRect( 80, 12, 48, 8, BLACK );
    display.setTextSize(1);
    display.setCursor(80, 12); display.print( "..." );
    
    if( enc1.wasTurned() || requireUpdate ) {
      requireUpdate = false;
      menuSensorsDHT.update( enc1.getDelta() );
      display.fillRect( 0, 23, 45, 41, BLACK );
        
//Menu.draw( _selector('>'), _x(0), _y(0), _dY(0), elemens2Show(5),
//          state(SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT)[
//            HIDE_SELECTOR, SHOW_BIG_SELECT, SHOW_NUM_LINE, SHOW_NUM_LINE_SELECT
//          ] )
      menuSensorsDHT.draw( '>', 0, 23, 2, 4, 0 );
      display.display();
    }
    

    //
    display.setTextSize(2);
    display.setCursor( 62, 25 );  display.print( cstrCheck );
    display.setCursor( 62, 44 );  display.print( cstrCheck );
    display.display();
    
    dht.setup( PIN_2, dhtTypes[menuSensorsDHT.id] );
    
    display.setTextSize(1);
    display.setCursor(80, 12); display.print( dht.getStatusString() );
    display.display();
    
    if( dhtTypes[menuSensorsDHT.id] == DHT_light::AUTO_DETECT ) {
      menuSensorsDHT.id = (int)dht.getModel();
      requireUpdate = true;
    }
    
    ulDHTWaitFor = dht.getMinimumSamplingPeriod();
    ulLastDHT = millis();
    ulDHTWaitTo = ulLastDHT + ulDHTWaitFor;
    
    humid = 0; temp = 0; humidOld = 0; tempOld = 0;
    
    conDelay( ulDHTWaitFor, requireUpdate || exitCode || enc1.wasTurned() || ( millis() > ulDHTWaitTo ) );
    
    if( millis() >= ulDHTWaitTo ){
      
      do {
        display.fillRect( 80, 12, 48, 8, BLACK );
        display.setTextSize(1);
        display.setCursor(80, 12); display.print( "..." );
        display.display();
        
        conDelay( 1000, exitCode || enc1.wasTurned() );
        humid = dht.getHumidity();     // [%]
        temp = dht.getTemperature();   // [Celsius]
        
        display.setCursor(80, 12); display.print( dht.getStatusString() );
        display.setTextSize(2);
        display.setCursor( 62, 25 );
        if( isnan(temp) ) display.print( cstrFail );
        else {
          temp = 0.2*temp + 0.8*tempOld;
          tempOld = temp;
          display.print( String(temp, 1) );
        }
  
        display.setCursor( 62, 44 );
        if( isnan(temp) ) display.print( cstrFail );
        else {
          humid = 0.2*humid + 0.8*humidOld;
          humidOld = humid;
          display.print( String(humid, 1) );
        }
        
        display.display();
      } while( !( exitCode || enc1.wasTurned() || requireUpdate ) ) ;
    }
  } while( !exitCode ) ;
  
  exitCode = false;
  clearPins();
}










// +----------------------+
// | DST - HC-SR04 sensor |
// +----------------------+
//  T1    T2    T3    PWM_OUT
//  1     2     3     4
//  VCC   Trig  Echo  GND

#define PIN_HCSR_TRIG PIN_2
#define PIN_HCSR_ECHO PIN_3

String HC_SR_Status[] = { "Ok", "Not connected", "Error" };

//float dst_in_us = 0.03438f; // v_wave/100000 [cm], 20C

// returns value in [mm]
long HC_SR04_getEcho( uint32_t pinTrig, uint32_t pinEcho, unsigned long _timeout = 23530 ) {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(5);   digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);  digitalWrite(pinTrig, LOW);
  
  pinMode(pinEcho, INPUT);
  return pulseIn(pinEcho, HIGH, _timeout); // max dst 4 [m] ~ 23.53 [ms]
}



//##
void loop_HC_SR04() {
  onEnterSubLoop();
  long dT = 0, dst = 0;
  uint8_t uiErrorHC_SR = 0;
  unsigned long maxTimeout_HC_SR04 = 23530;
  
  display.setTextSize( 1 );
  display.setCursor( 0, 0 ); display.print( "Sensors HC-SR04" );
  
  display.setTextSize(2);
  display.setCursor(76, 19); display.print("[mm]");
  display.setTextSize(1);
  display.setCursor(78, 39); display.print("[us]");
  display.setCursor(0, 51); display.print("Status:");
  
  display.display();
  
  pinMode( PIN_1, OUTPUT ); pinMode( PIN_2, OUTPUT );
  pinMode( PIN_3, INPUT ); pinMode( PIN_PWM_OUT, OUTPUT );
  digitalWrite( PIN_1, HIGH );  digitalWrite( PIN_2, LOW );
  digitalWrite( PIN_PWM_OUT, LOW );
  
  do {
    dT = HC_SR04_getEcho( PIN_HCSR_TRIG, PIN_HCSR_ECHO, maxTimeout_HC_SR04 );
    
    if( dT > 0 && dT < maxTimeout_HC_SR04 ) uiErrorHC_SR = 0; // OK
    else if( dT == 0 )                      uiErrorHC_SR = 1; // not connected ( pulseIn() = 0 )
    else                                    uiErrorHC_SR = 2; // Error
    
    dst = ( dT * 3438 ) / 20000; // [mm]

    display.fillRect( 42, 51, 86, 10, BLACK ); // clear status msg
    display.setTextSize(1);
    display.setCursor(34, 39);   display.print( fillPlacesLeft( (String)dT, ' ', 7 ) );
    display.setCursor(42, 51);  display.print( HC_SR_Status[ uiErrorHC_SR ] );
    
    display.setTextSize(2);
    display.setCursor(4, 19); display.print( fillPlacesLeft( (String)dst, ' ', 6 ) );
    display.display();

    conDelay( 100, exitCode );
    
  } while( !exitCode ) ;
  
  exitCode = false;
  clearPins();
}
//##



///////////////////////

#define NUM_SENSOR_PAGES 3
String strSensors[] = { "DHT11/21/22", "HC-SR04", "LM35" };
Menu menuSensors( strSensors, NUM_SENSOR_PAGES );

void (*pagesSensors[NUM_SENSOR_PAGES])() {
  loop_DHT,     // DHT11/21/22
  loop_HC_SR04, // HC-SR04
  empty_loop    // LM35
};


void loop_sensors() {
  onEnterSubLoop();
  
  bool requireUpdate = true;
  
  enc1.getDelta(); // erase turns from previous screens
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print( "Sensor tester" );
  display.display();
  
  while( !exitCode ) {
          
    if( enc1.wasTurned() || requireUpdate ) {
      requireUpdate = false;
      menuSensors.update( enc1.getDelta() );
      display.fillRect( 0, 8, 128, 56, BLACK );
      
//Menu.draw( _selector('>'), _x(0), _y(0), _dY(0), elemens2Show(5),
//          state(SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT)[
//            HIDE_SELECTOR, SHOW_BIG_SELECT, SHOW_NUM_LINE, SHOW_NUM_LINE_SELECT
//          ] )
      menuSensors.draw( '>', 0, 10, 2, 5, SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT );
      display.display();
    }
    
    if( btnEnc.wasPressed() ) {
      pagesSensors[ menuSensors.id ]();
      requireUpdate = true;
    }
    
    conDelay( 1000, exitCode || enc1.wasTurned() );
  }
  
  onExitSubLoop();
}
