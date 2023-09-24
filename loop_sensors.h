// +-----------------------+
// | Sensor tester Section |
// +-----------------------+

#include <DHT.h>

const String cstrFail = "FAIL";
const String cstrCheck = "?   ";

const uint8_t dhtTypes[] = {DHT11, DHT21, DHT22};
//  T1    T2    T3    PWM_OUT
//  1     2     3     4
//  VCC   DATA  NC    GND


#define NUM_DHT_PAGES 3
String strSensorsDHT[] = { "DHT11", "DHT21", "DHT22" };
Menu menuSensorsDHT( strSensorsDHT, NUM_DHT_PAGES );

void loop_DHT() {
  onEnterSubLoop();
  int8_t selectedType = 0;
  float humid = 0, temp = 0, humidOld = 0, tempOld = 0;
  bool requireUpdate = true;
  
  display.setTextSize( 1 );
  display.setCursor( 0, 0 ); display.print( "Sensors DHT" );
  display.setCursor(6, 12); display.print("Type:");
  
  display.setTextSize(2);
  display.setCursor(115, 19); display.print("C");
  display.setCursor(115, 38); display.print("%");

  display.drawBitmap(49, 18, BMP_TEMP, 8, 16, 1);
  display.drawBitmap(47, 37, BMP_HUMID, 12, 16, 1);
  display.display();
  
  pinMode( PIN_1, OUTPUT ); pinMode( PIN_2, OUTPUT );
  pinMode( PIN_3, OUTPUT ); pinMode( PIN_PWM_OUT, OUTPUT );
  digitalWrite( PIN_1, HIGH );  digitalWrite( PIN_2, HIGH );
  digitalWrite( PIN_3, LOW );   digitalWrite( PIN_PWM_OUT, LOW );
  
  do {
    /*
    if( enc1.wasTurned() ) {
      requireUpdate = true;
      selectedType += enc1.getDelta();
      if( selectedType < 0 ) selectedType = 0;
      else if( selectedType > 2 ) selectedType = 2;
    }

    if(requireUpdate) {
      requireUpdate = false;
      display.fillRect( 0, 23, 6, 53, BLACK );
      display.setTextSize(1);
      display.setCursor( 0, 23 + 11*selectedType );  display.print(">");
      display.setTextSize(2);
      display.setCursor( 62, 19 );  display.print( cstrCheck );
      display.setCursor( 62, 38 );  display.print( cstrCheck );
      display.display();
    }
    */

    if( enc1.wasTurned() || requireUpdate ) {
      requireUpdate = false;
      menuSensorsDHT.update( enc1.getDelta() );
      display.fillRect( 0, 23, 45, 41, BLACK );
        
//Menu.draw( _selector('>'), _x(0), _y(0), _dY(0), elemens2Show(5),
//          state(SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT)[
//            HIDE_SELECTOR, SHOW_BIG_SELECT, SHOW_NUM_LINE, SHOW_NUM_LINE_SELECT
//          ] )
      menuSensorsDHT.draw( '>', 6, 23, 2, 5, 0 );
      display.display();
    }
    

    //
    DHT dht(PIN_2, dhtTypes[selectedType]);
    dht.begin();
    humid = 0; temp = 0; humidOld = 0; tempOld = 0;
    display.setTextSize(2);
    do {
      conDelay( 1000, exitCode || enc1.wasTurned() );
      humid = dht.readHumidity();     // [%]
      temp = dht.readTemperature();   // [Celsius]
      
      display.setCursor( 62, 19 );
      if( isnan(temp) ) display.print( cstrFail );
      else {
        temp = 0.2*temp + 0.8*tempOld;
        display.print( String(temp, 1) );
      }

      display.setCursor( 62, 38 );
      if( isnan(temp) ) display.print( cstrFail );
      else {
        humid = 0.2*humid + 0.8*humidOld;
        display.print( String(humid, 1) );
      }
      
      display.display();
    } while( !( exitCode || enc1.wasTurned() ) ) ;
  } while( !exitCode ) ;
  
  exitCode = false;
  clearPins();
}







///////////////////////

#define NUM_SENSOR_PAGES 3
String strSensors[] = { "DHT11/21/22", "HC-04", "LM35" };
Menu menuSensors( strSensors, NUM_SENSOR_PAGES );

void (*pagesSensors[NUM_SENSOR_PAGES])() {
  loop_DHT,    // DHT11/21/22
  empty_loop,    // HC-04
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
