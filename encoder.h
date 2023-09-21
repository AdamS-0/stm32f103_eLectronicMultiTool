// +-----------------+
// | Encoder section |
// +-----------------+

// FSM
// 0 (0, 1, 1) - Idle
// CLK
// 1 (0, 0, 1)
// 2 (0, 0, 0)
// 3 (0, 1, 0)

// CCLK
// 4 (1, 1, 0)
// 5 (1, 0, 0)
// 6 (1, 0, 1)

#include <FSMEncoder.h>
FSMEncoder enc1(PIN_ENC_A, PIN_ENC_B);

void INT_PIN_ENC_A() { enc1.interrupt_CLK(); }
void INT_PIN_ENC_B() { enc1.interrupt_DT(); }



// only unsigned types are legal!!
int8_t u8_encChangePos = 0;
bool b_encChangePlace = true;
void encoderChangeClear() {
  u8_encChangePos = 0;
  b_encChangePlace = true;
}

template<typename T>
void encoderBlinkValue( uint8_t x, uint8_t y, T &v, int textSize = 1, int baseSystem = 10, uint8_t places = 0 ) {
  String strVal = val2str(v, baseSystem, places, '0');
  places = strVal.length();
  display.setCursor(x, y);
  display.setTextSize(textSize);
  display.print(strVal);

  uint8_t yi = y + textSize*8 - 1;
  uint8_t xi = x + (places - u8_encChangePos - 1)*textSize*6;
  display.drawFastHLine(xi, yi, textSize*6, (millis() % 1000 > 500) && !b_encChangePlace ? BLACK : WHITE);
}



template<typename T>
void encoderChangeValue(T &v, int baseSystem = 10, T maxValue = 0) {
  if( baseSystem == 0 ) return;
  
  // change from place to digit modification!
  if( b_encBtn ) {
    b_encBtn = false;
    b_encChangePlace = !b_encChangePlace;
  }

  if( !enc1.wasTurned() ) return;

  // should get all ones, e.g. 11111111 or maxValue
  maxValue = maxValue == 0 ? maxValue - 1 : maxValue;

  int8_t dV = enc1.getDelta();
  
  if( b_encChangePlace ) {
    // change place (ends with return!)
    uint8_t plcs = 0;
    T mxV = maxValue;
    while( mxV > 0 ) {
      mxV /= baseSystem;
      plcs++;
    }
    
    u8_encChangePos = u8_encChangePos - dV;
    if( u8_encChangePos < 0 ) u8_encChangePos = 0;
    else if( u8_encChangePos >= plcs ) u8_encChangePos = plcs - 1;
    
    return;
  }
  
  // change digit
  T basePosPow = pow( baseSystem, u8_encChangePos );
  dV = dV % baseSystem;
  T oldDigit = v / basePosPow;
  oldDigit = oldDigit % baseSystem;

  T baseV = v - oldDigit * basePosPow;
  T maxDigit = ( maxValue - baseV ) / basePosPow;
  maxDigit = maxDigit >= baseSystem ? baseSystem - 1 : maxDigit;
    
  T newDigit = oldDigit + dV;
  if( dV > 0 )  newDigit = min( newDigit, maxDigit );
  else if( abs(dV) < oldDigit ) newDigit = oldDigit + dV;
  else newDigit = 0;
  // newDigit in range <0, maxDigit)
  
  Serial.println( "oD:" + (String)oldDigit + ", nD:" + (String)newDigit + ", mxD:" + (String)maxDigit );
  
  T newV = v;
  newV -= oldDigit * basePosPow;
  newV += newDigit * basePosPow;
  v = newV;
}

// v = 325
// base = 10
// pos = 1
// 
// oldV = 325 / ( base^pos )
// oldV =  32 % base
// oldV =   2
//   
// 325 - oldV * base^pos
// 325 - 2*10^1
// 305
// 
// new = 2 + dV
// 305 + new*base^pos
