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

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

class DHT_light {
public:

  typedef enum {
    AUTO_DETECT,
    DHT11,
    DHT22,
    AM2302,  // Packaged DHT22
    RHT03    // Equivalent to DHT22
  }
  DHT_MODEL_t;

  typedef enum {
    ERROR_NONE = 0,
    ERROR_TIMEOUT,
    ERROR_CHECKSUM,
	ERROR_TOO_FAST
  }
  DHT_ERROR_t;
  
  DHT_light() {}
  void setup(uint32_t pin, DHT_MODEL_t model=AUTO_DETECT);
  void resetTimer();

  float getTemperature();
  float getHumidity();

  DHT_ERROR_t getStatus() { return error; };
  const char* getStatusString();

  DHT_MODEL_t getModel() { return model; }

  int getMinimumSamplingPeriod() { return model == DHT11 ? 1000 : 2000; }

  int8_t getNumberOfDecimalsTemperature() { return model == DHT11 ? 0 : 1; };
  int8_t getLowerBoundTemperature() { return model == DHT11 ? 0 : -40; };
  int8_t getUpperBoundTemperature() { return model == DHT11 ? 50 : 125; };

  int8_t getNumberOfDecimalsHumidity() { return 0; };
  int8_t getLowerBoundHumidity() { return model == DHT11 ? 20 : 0; };
  int8_t getUpperBoundHumidity() { return model == DHT11 ? 90 : 100; };

  static float toFahrenheit(float fromCelcius) { return 1.8 * fromCelcius + 32.0; };
  static float toCelsius(float fromFahrenheit) { return (fromFahrenheit - 32.0) / 1.8; };

protected:
  void readSensor();

  float temperature;
  float humidity;

  uint32_t pin;

private:
  DHT_MODEL_t model;
  DHT_ERROR_t error;
  unsigned long lastReadTime = 0;
};



void DHT_light::setup(uint32_t _pin, DHT_MODEL_t _model) {
  this->pin = _pin;
  this->model = _model;
  error = ERROR_NONE;
  //this->resetTimer(); // Make sure we do read the sensor in the next readSensor()
  if( model == AUTO_DETECT) {
    this->model = DHT22;
    readSensor();
	if( error == ERROR_TOO_FAST ) delay( 2001 - ( millis() - lastReadTime ) );
	readSensor();
	
    if( error == ERROR_TIMEOUT ) {
      this->model = DHT11;
      // Warning: in case we auto detect a DHT11, you should wait at least 1000 msec
      // before your first read request. Otherwise you will get a time out error.
    }
  }
  error = ERROR_NONE;
}

void DHT_light::resetTimer() {
  this->lastReadTime = millis();
}

float DHT_light::getHumidity() {
  
  if( (unsigned long)(millis() - lastReadTime) > (model == DHT11 ? 1000L : 2000L) )
    readSensor();
  
  return humidity;
}

float DHT_light::getTemperature() {
  if( (unsigned long)(millis() - lastReadTime) >= (model == DHT11 ? 1000L : 2000L) )
    readSensor();
  return temperature;
}

const char* DHT_light::getStatusString() {
  switch( error ) {
    case DHT_light::ERROR_TIMEOUT:
      return "TIMEOUT";

    case DHT_light::ERROR_CHECKSUM:
      return "CHECKSUM";
	  
	case DHT_light::ERROR_TOO_FAST:
      return "TOO FAST";

    default:
      return "OK";
  }
}


void DHT_light::readSensor() {
  // Make sure we don't poll the sensor too often
  // - Max sample rate DHT11 is 1 Hz   (duty cicle 1000 ms)
  // - Max sample rate DHT22 is 0.5 Hz (duty cicle 2000 ms)
  unsigned long startTime = millis();
  if( (unsigned long)(startTime - lastReadTime) < (model == DHT11 ? 1000L : 2000L) ) {
    error = ERROR_TOO_FAST;
	return;
  }
  
  delay(100);
  lastReadTime = startTime;
  error = ERROR_NONE;
  temperature = NAN;
  humidity = NAN;
  
  // Request sample
  pinMode(pin, INPUT_PULLUP);	delay(1);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW); // Send start signal
  if( model == DHT11 ) delay(20);
  else                 delayMicroseconds(1100); // This will fail for a DHT11 - that's how we can detect such a device

  pinMode(pin, INPUT_PULLUP); // Switch bus to receive data

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
