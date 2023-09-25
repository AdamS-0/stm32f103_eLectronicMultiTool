/******************************************************************
  DHT Temperature & Humidity Sensor library for Arduino.

  Features:
  - Support for DHT11 and DHT22/AM2302/RHT03
  - Auto detect sensor model
  - Very low memory footprint
  - Very small code

  http://www.github.com/markruys/arduino-DHT

  Written by Mark Ruys, mark@paracas.nl.

  BSD license, check license.txt for more information.
  All text above must be included in any redistribution.

  Datasheets:
  - http://www.micro4you.com/files/sensor/DHT11.pdf
  - http://www.adafruit.com/datasheets/DHT22.pdf
  - http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Weather/RHT03.pdf
  - http://meteobox.tk/files/AM2302.pdf

  Changelog:
   2013-06-10: Initial version
   2013-06-12: Refactored code
   2013-07-01: Add a resetTimer method
 ******************************************************************/

#include "DHT_light.h"

void DHT_light::setup(uint32_t _pin, DHT_MODEL_t _model) 
  //Serial.println("0");
  this->pin = pin;
  this->model = model;
  this->resetTimer(); // Make sure we do read the sensor in the next readSensor()
  //Serial.println("1");
  if ( model == AUTO_DETECT) {
    //Serial.println("2");
    this->model = DHT22;
    //readSensor();
    //Serial.println("3");
    if ( error == ERROR_TIMEOUT ) {
      this->model = DHT11;
      // Warning: in case we auto detect a DHT11, you should wait at least 1000 msec
      // before your first read request. Otherwise you will get a time out error.
    }
    //Serial.println("Error::_ " + (String)error);
  }
  //Serial.println("4");
}

void DHT_light::resetTimer() {
  this->lastReadTime = millis() - 3000;
}

float DHT_light::getHumidity() {
  readSensor();
  return humidity;
}

float DHT_light::getTemperature() {
  readSensor();
  return temperature;
}

const char* DHT_light::getStatusString() {
  switch( error ) {
    case DHT_light::ERROR_TIMEOUT:
      return "TIMEOUT";

    case DHT_light::ERROR_CHECKSUM:
      return "CHECKSUM";

    default:
      return "OK";
  }
}


void DHT_light::readSensor() {
  // Make sure we don't poll the sensor too often
  // - Max sample rate DHT11 is 1 Hz   (duty cicle 1000 ms)
  // - Max sample rate DHT22 is 0.5 Hz (duty cicle 2000 ms)
  unsigned long startTime = millis();
  if( (unsigned long)(startTime - lastReadTime) < (model == DHT11 ? 999L : 1999L) )
    return;
  
  lastReadTime = startTime;

  temperature = 0;
  humidity = 0;

  // Request sample
  digitalWrite(pin, LOW); // Send start signal
  pinMode(pin, OUTPUT);
  if( model == DHT11 ) delay(18);
  else                 delayMicroseconds(800); // This will fail for a DHT11 - that's how we can detect such a device

  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH); // Switch bus to receive data

  // We're going to read 83 edges:
  // - First a FALLING, RISING, and FALLING edge for the start bit
  // - Then 40 bits: RISING and then a FALLING edge per bit
  // To keep our code simple, we accept any HIGH or LOW reading if it's max 85 usecs long

  uint16_t rawHumidity = 0;
  uint16_t rawTemperature = 0;
  uint16_t data = 0;

  for( int8_t i = -3 ; i < 2 * 40; i++ ) {
    byte age;
    startTime = micros();

    do {
      age = (unsigned long)(micros() - startTime);
      if( age > 90 ) {
        error = ERROR_TIMEOUT;
        return;
      }
    }
    while( digitalRead(pin) == (i & 1) ? HIGH : LOW ) ;

    if( i >= 0 && (i & 1) ) {
      // Now we are being fed our 40 bits
      data <<= 1;

      // A zero max 30 usecs, a one at least 68 usecs.
      if( age > 30 ) data |= 1; // we got a one
    }

    switch( i ) {
      case 31:
        rawHumidity = data;
        break;
      case 63:
        rawTemperature = data;
        data = 0;
        break;
    }
  }

  // Verify checksum

  if( (byte)(((byte)rawHumidity) + (rawHumidity >> 8) + ((byte)rawTemperature) + (rawTemperature >> 8)) != data ) {
    error = ERROR_CHECKSUM;
    return;
  }

  // Store readings

  if( model == DHT11 ) {
    humidity = rawHumidity >> 8;
    temperature = rawTemperature >> 8;
  } else {
    humidity = rawHumidity * 0.1;

    if( rawTemperature & 0x8000 )
      rawTemperature = -(int16_t)(rawTemperature & 0x7FFF);
	
    temperature = ((int16_t)rawTemperature) * 0.1;
  }

  error = ERROR_NONE;
}
