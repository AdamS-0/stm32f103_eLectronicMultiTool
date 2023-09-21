float getCap( uint8_t iA, uint8_t iB );
float getRes( uint8_t iA, uint8_t iB );
float getVolt( uint8_t iA, uint8_t iB );




// tester seciotn
void test_RES() {
  if( exitCode ) return;
  onEnterSub_tester();
  display.drawBitmap(20, 1, BMP_RES_ICON, 16, 5, WHITE);
  display.drawBitmap(42, 43, BMP_RES, 44, 14, WHITE);
  display.display();
  
  //pins are disconnected due to clearPins()

  bool found_pins = false, last_found = false;
  float R = 0;
  unsigned long nums = 0;

  fiR.set();
  while( !exitCodeSub & !exitCode ) {
    R = 0;
    // 1. find 2 pins
    // VCC o--[Rx]--+--[Rknown]--GND
    // 2. measure voltage in the mid point
    // 3. calculate resistance: Rx = Rknown * ( 4095 / aIn - 1 )
    // 4. show results
    
    last_found = found_pins;
    found_pins = false;
    
    // 1. find 2 pins
    clearPins();
    uint8_t iA, iB;
    for( iA = 0; iA < 3 && !found_pins; iA++ ) {
      for( iB = 0; iB < 3 && !found_pins; iB++ ) {
        if( iB == iA ) continue;
        
        clearPins();
        
        pinA = pins123[ iA ]; pinB = pins123[ iB ];
        pinMode( pinA, INPUT ); pinMode( pinB, OUTPUT );
        digitalWrite( pinB, HIGH );
        
        int16_t AIn = analogRead( pinA );
        // AIn should be > 3900
        if( AIn > 3900 ) {
          found_pins = true;
          break;
        }
      }
      
      if( found_pins ) break;
    }
    
    if( !found_pins && last_found ) { fiR.set(); nums = 0; }
    
    print_pin_numbers( iA, iB );
    display.display();
    
    if( found_pins ) {
      // 2. measure voltage in the mid point
      R = getRes( iA, iB );
      if( R < 200 ) R = (R*R)/300.0f;
      // 1-3, 2-3
      if( iB == 2 )  {
//        Serial.print( "Rold = " + (String)R );
//        if( R < 300 ) R = 0.07f*( R - 200 )*8 + 40;
//        else if( R < 500 ) R = 0.9f*( R - 210 )*1 + 10;
//        else if( R < 1300 ) R = 0.8f*( R - 500 )*1 + 300;
//        else if( R < 3000 ) R = 0.95f*( R - 1300 )*1 + 1200;
//        else if( R < 4000 ) R = 1.0f*( R - 100 )*1 + 0;
//        else if( R < 12000 ) R = 1.0f*( R - 400 )*1 + 0;
//        else R = R = 0.97f*( R - 0 )*1 + 0;

//        Serial.println( ",\tR = " + (String)R );
      }
      
      fiR.push( R );
      if( nums >= LEN_FIL_R ) R = fiR.get();

      if( R < 1000 ) R = 10*round( R/10 );
      
      nums++;
    }
    
    // 4. show results
    printValue( R, char( 0x7F ), R > 1000 );
    
    display.display();
    mdelay( 100 );
  }

}




void test_DIODE() {
  if( exitCode ) return;
  onEnterSub_tester();
  display.drawBitmap(38, 1, BMP_DIODE_ICON, 16, 5, WHITE);
  display.drawBitmap(42, 43, BMP_DIODE, 44, 14, WHITE);
  display.display();
  
  //pins are disconnected due to clearPins()
  
  float V = 0;
  bool found_pins = false, last_found = false;
  
  fiR.set();
  while( !exitCodeSub & !exitCode ) {
    V = 0;
    // 1. find 2 pins
    // pinA (VCC) o--[A-Diode-K]--(pinB)--[Rknown]--o pinB_support_Ri (GND)
    // 2. measure voltage in the mid point
    // 3. calculate voltage: V = 3.3f * ( 1 - V / 4095 );
    
    // 1. find 2 pins
    
    // 1-2, 1-3
    // 2-1, 2-3
    // 3-1, 3-2
    last_found = found_pins;
    found_pins = false;
    
    // 1. find 2 pins
    clearPins();
    
    uint8_t iA, iB;
    for( iA = 0; iA < 3 && !found_pins; iA++ ) {
      for( iB = 0; iB < 3 && !found_pins; iB++ ) {
        if( iB == iA ) continue;
        
        clearPins();
        
        pinA = pins123[ iA ]; pinB = pins123[ iB ];
        pinMode( pinA, OUTPUT ); pinMode( pinB, INPUT_PULLDOWN );
        digitalWrite( pinA, HIGH );
        
        int16_t BIn = analogRead( pinB );
        //Serial.println( (String)iA + "->" + (String)iB + ", aIn: " + (String)BIn );
        
        // AIn should be > 800
        if( BIn > 800 ) {
          found_pins = true;
          break;
        }
        
      }
      
      if( found_pins ) break;
    }
    
    if( !found_pins && last_found ) fiR.set();
    
    // find again, but start from 1-2, not in the middle
    if( found_pins && !last_found ) continue;
    
    print_pin_numbers( iA, iB );
    display.display();
    
    if( found_pins ) {
      V = getVolt( iA, iB );
      uint32_t pA = pins123[ iA ], pB = pins123[ iB ];
      pinMode( pA, OUTPUT ); pinMode( pB, OUTPUT );
      digitalWrite( pA, HIGH );
      digitalWrite( pB, LOW );
    }
    // 4. show results
    printValue( V, 'V' );
    
    display.display();
    mdelay( 500 );
  }

}




void test_CAP() {
  if( exitCode ) return;
  onEnterSub_tester();
  display.drawBitmap(56, 1, BMP_CAP_ICON, 16, 5, WHITE);
  display.drawBitmap(42, 43, BMP_CAP, 44, 14, WHITE);
  
  printValue( 0, 'F' );
  display.display();
  
  //pins are disconnected due to clearPins()
  
  // wait 1s before test
  //while( !exitCodeSub & !exitCode && ( millis() - ul_last_btn_B ) < 1000UL ) delay( 50 );
  
  float C = 0;
  bool found_pins = false, last_found = false;
  
  while( !exitCodeSub & !exitCode ) {
    C = 0;
    // 1. find 2 pins
    // measure Capacitance C = getCap(...):
    //  2. first measurement with small R, R1 = 470 Ohm
    //  2.1. if expired, go to 4.
    //  2.2. if fast enough ( T > 470 us ) go to 3.
    //  2.3. second measurement with big R, R2 = 2 MOhm, more precise, about 2 seconds
    // 3. calculate capacitance
    // 4. show results
    
    // 1. find 2 pins
    
    // 1-2, 1-3
    // 2-1, 2-3
    // 3-1, 3-2
    last_found = found_pins;
    found_pins = false;
    
    clearPins();
    uint8_t iA, iB;
    for( iA = 0; iA < 3 && !found_pins; iA++ ) {
      for( iB = 0; iB < 3 && !found_pins; iB++ ) {
        if( iB == iA ) continue;
        
        clearPins();
        
        pinA = pins123[ iA ]; pinB = pins123[ iB ];
        pinMode( pinA, INPUT ); pinMode( pinB, OUTPUT );
        pinMode( pinsSupport[ 2*iA ], OUTPUT );
        digitalWrite( pinB, LOW );
        
        int16_t oldAIn = analogRead( pinA );
        digitalWrite( pinsSupport[ 2*iA ], LOW ); // LOW to low resistance - fast discharging
        mdelay( 100 );
        pinOff( pinsSupport[ 2*iA ] );
        int16_t AIn = analogRead( pinA );
        
        // AIn should be 0 V
        if( oldAIn < 10 && AIn < 30 ) {
          found_pins = true;
          break;
        }
      }
      
      if( found_pins ) break;
    }
    
    // find again, but start from 1-2, not in the middle
    if( found_pins && !last_found ) continue;
    
    print_pin_numbers( iA, iB );
    display.display();
    
    if( found_pins ) {
      C = getCap( iA, iB );
    }
    // 4. show results
    printValue( C, 'F' );
    
    display.display();
    mdelay( 500 );
  }

}




void test_TR() {
  if( exitCode ) return;
  onEnterSub_tester();
  display.drawBitmap(74, 1, BMP_TR_ICON, 16, 5, WHITE);
  display.display();
  
  //pins are disconnected due to clearPins()
  
  while( !exitCodeSub & !exitCode ) {
    //exitCodeSub = true;


    display.display();
    mdelay( 1000 );
  }

}




void test_IND() {
  if( exitCode ) return;
  onEnterSub_tester();
  display.drawBitmap(92, 1, BMP_IND_ICON, 16, 5, WHITE);
  display.drawBitmap(42, 45, BMP_IND, 44, 10, WHITE);
  display.display();

  while( !exitCodeSub & !exitCode ) {
    //exitCodeSub = true;


    display.display();
    mdelay( 1000 );
  }

}

















#define NUM_R_TESTS 40
uint16_t R_tab[NUM_R_TESTS];
float Rmean = 0, stdDev = 0;

float getRes( uint8_t iA, uint8_t iB ) {
  // pinA (VCC) o--[Rx]--(pinB)--[Rknown]--o pinB_support_Ri (GND)
  // 2. measure voltage in the mid point
  // 3. calculate resistance: Rx = Rknown * ( 4095 / aIn - 1 )
  
  uint32_t pA = pins123[ iA ], pB = pins123[ iB ];
  
  float R = 0, R2 = 0, Rknown, tresholdR = 1000; //1 [kOhm]
  uint32_t first_try_pin = pinsSupport[ 2*iB ], second_try_pin = pinsSupport[ 2*iB + 1 ];
  uint8_t i = 0;
  
  Rknown = (float)pinsSupportR[ 2*iB ]; // 470 ?
  
  clearPins();
  pinMode( pA, OUTPUT );
  pinMode( pB, INPUT_ANALOG );
  pinMode( first_try_pin, OUTPUT );
  
  digitalWrite( first_try_pin, LOW ); digitalWrite( pA, HIGH );
  uint8_t rId = 0;
//  for( i = 0; i < 20; i++ ) {
//    delay( 5 );
//    //R += analogRead( pB );
//    R = analogRead( pB ); // uint16_t(analogRead( pB )/4) * 4;
//    fiR.push( R );
//  }
  //R = R / i;

  // Standard deviation
  Rmean = 0; stdDev = 0; R = 0;
  for( rId = 0; rId < NUM_R_TESTS; rId++ ) {
    delay( 2 );
    R_tab[rId] = ( (int)analogRead( pB )/1 ) * 1;
    Rmean += R_tab[rId];
  }
  Rmean /= NUM_R_TESTS;

  
  for( rId = 0; rId < NUM_R_TESTS; rId++ ) {
    stdDev += pow(R_tab[rId] - Rmean, 2);
  }
  stdDev = sqrt( stdDev / NUM_R_TESTS );

  int8_t counts = 0;
  for( rId = 0; rId < NUM_R_TESTS; rId++ ) {
    if( abs( R_tab[rId] - Rmean ) <= 1.1f*stdDev ) {
      R += R_tab[rId];
      counts++;
    }
  }
  R /= counts;
  
  
  pinOff( first_try_pin );
  //R = fiR.get();
  R2 = R;
  //R = Rknown * ( 4095 / R - 1 ); // 3960
  //R = 1.076 * ( R - Rknown ) + Rknown;
  //R = 3.7f * Rknown / ( R * 3.3f / 4095 ) - Rknown;
  R = Rknown * ( 4095 / R - 1 );
  
  // smaller R
  // -----
  // bigger R
  
  if( R > tresholdR ) {
    //fiR.set();
    //R2 = R; R = 0;
    pinMode( second_try_pin, OUTPUT );
    digitalWrite( second_try_pin, LOW );
//    for( i = 0; i < 20; i++ ) {
//      delay( 5 );
//      //R += analogRead( pB );
//      //fiR.push( uint16_t(analogRead( pB )/2) * 2 );
//    }
    //R = R / i;
    //R = fiR.get();
    
    // Standard deviation
    Rmean = 0; stdDev = 0; R = 0;
    for( rId = 0; rId < NUM_R_TESTS; rId++ ) {
      delay( 2 );
      R_tab[rId] = ( (int)analogRead( pB )/1 ) * 1;
      
      Rmean += R_tab[rId];
    }
    Rmean /= NUM_R_TESTS;
  
    
    for( rId = 0; rId < NUM_R_TESTS; rId++ ) {
      stdDev += pow(R_tab[rId] - Rmean, 2);
    }
    stdDev = sqrt( stdDev / NUM_R_TESTS );
  
    int8_t counts = 0;
    for( rId = 0; rId < NUM_R_TESTS; rId++ ) {
      if( abs( R_tab[rId] - Rmean ) <= 1.1f*stdDev ) {
        R += R_tab[rId];
        counts++;
      }
    }
    R /= counts;
    
    pinOff( second_try_pin );
    
    Rknown = (float)pinsSupportR[ 2*iB + 1 ];
    //R = 3.7f * Rknown / ( R * 3.3f / 4095 ) - Rknown;
    R = Rknown * ( 4095 / R - 1 );
  }


  // VDD = 3.6 = URknown + UR
  // I*( Rknown + R ) = 3.6
  // I = R'ain /Rknown
  // ( Rknown + R ) * R'ain /Rknown = 3.6
  // ( Rknown + R ) = 3.6 * Rknown / R'ain
  // R = 3.6 * Rknown / R'ain - Rknown;
//  R = Rknown * ( 4095 / R - 1 );
//  R = 3.7 * Rknown / R - Rknown;

  // R = U/I
  // Rknown = R'ain / I
  // I = R'ain / Rknown
  // I = VDD / ( R + Rknown )
  // R'ain / Rknown = VDD / ( R + Rknown )
  // R + Rknown = VDD * Rknown / R'ain
  // R = 3.7 * Rknown / R'ain - Rknown
  // R = 3.7 * Rknown / ( R'ain * 3.3 / 4095 ) - Rknown
  Serial.println( "mean(R) = " + (String)Rmean + ";\tstdDev(R) = " + (String)stdDev + ";\tcounts = " + (String)counts + ";\tmean(R2) = " + (String)R2 + ";\tR = " + (String)R + ";\tRknown = " + (String)Rknown );
  
  return R;
}










float getVolt( uint8_t iA, uint8_t iB ) {
  // pinA (VCC) o--[A-Diode-K]--(pinB)--[Rknown]--o pinB_support_Ri (GND)
  // 2. measure voltage in the mid point
  // 3. calculate voltage: V = 3.3f * ( 1 - V / 4095 );
  
  uint32_t pA = pins123[ iA ], pB = pins123[ iB ];
  
  float V = 0;
  uint32_t first_try_pin = pinsSupport[ 2*iB ], second_try_pin = pinsSupport[ 2*iB ];
  uint8_t i = 0;
  
  clearPins();
  pinMode( pA, OUTPUT );
  pinMode( pB, INPUT_ANALOG );
  
  
  pinMode( first_try_pin, OUTPUT );
  digitalWrite( first_try_pin, LOW ); digitalWrite( pA, HIGH );
  for( i = 0; i < 10; i++ ) {
    delay( 2 );
    //R += analogRead( pB );
    fiR.push( uint16_t(analogRead( pB )/4) * 4 );
  }
  pinOff( first_try_pin );
  V = fiR.get();
  
  
  pinMode( second_try_pin, OUTPUT );
  digitalWrite( second_try_pin, LOW ); digitalWrite( pA, HIGH );
  for( i = 0; i < 10; i++ ) {
    delay( 2 );
    //R += analogRead( pB );
    fiR.push( uint16_t(analogRead( pB )/4) * 4 );
  }
  pinOff( second_try_pin );
  V += fiR.get();
  V /= 2;
  
  
  V = 3.3f * ( 1 - V / 4095 );
  
  /*
  //if( V <= 1.4f ) {
    if( iA == 2 || iB == 2 ) V *= 0.55;
    else V *= 0.72;
  //}
  */
  
  return V;
}






void dischargeCap( uint32_t pA, uint32_t dischargePin, unsigned long maxTime = 10000000UL ) {
  unsigned long t = micros();
  digitalWrite( dischargePin, LOW );
  while( analogRead( pA ) > 2 && micros() - t < maxTime && !exitCodeSub ) delay(10);
  delay( 50 );
}




// pA - anode
// pB - kathode
float getCap( uint8_t iA, uint8_t iB ) {
  // 2. first measurement with small R, R1 = 470 Ohm
  // 2.1. if expired, go to 4.
  // 2.2. if fast enough ( T > 470 us ) go to 3.
  // 2.3. second measurement with big R, R2 = 2 MOhm, more precise, about 2 seconds
  // 3. calculate capacitance
  
  // 0.63 * 4095 = 2579.85 || 0.63 * 2047 = 1289.61 || 1024 * 0.63 = 645.12
  const uint16_t treshold = 2580;
  uint32_t pA = pins123[ iA ], pB = pins123[ iB ];
  
  float C = 0, R = 0;
  //bool time_expired = false;
  //unsigned long time_max = 250000000, time_max_discharge = 250000000; // [us]
  unsigned long T, T2;
  uint32_t first_try_pin = pinsSupport[ 2*iA ], second_try_pin = pinsSupport[ 2*iA + 1 ];
  
  
  clearPins();
  
  R = (float)pinsSupportR[ 2*iA ];
  pinMode( pA, INPUT );
  pinMode( pB, OUTPUT );
  pinMode( first_try_pin, OUTPUT );
  digitalWrite( pB, LOW );
  
  dischargeCap( pA, first_try_pin ); // discharge
  
  // charge
  digitalWrite( first_try_pin, HIGH );
  T = micros();
  while( analogRead( pA ) < treshold && !exitCodeSub ) ;
  T = micros() - T;
  
  dischargeCap( pA, first_try_pin ); // discharge

  // second measurement for smaller capacitance
  if( T < 470UL ) {
    T2 = T;
	pinOff( first_try_pin );
    pinMode( second_try_pin, OUTPUT );
    
    // charge
    digitalWrite( second_try_pin, HIGH );
    T = micros();
    while( analogRead( pA ) < treshold && !exitCodeSub ) ;
    T = micros() - T;
    
    digitalWrite( second_try_pin, LOW );
    pinOff( second_try_pin );
    
    pinMode( first_try_pin, OUTPUT );
    dischargeCap( pA, first_try_pin ); // discharge
    
    R = (float)pinsSupportR[ 2*iA + 1 ];
  }
  
  C = float(T / R)/1000000 ; // [F]
  return C;
}



// tester
void loop_tester() {
  onEnterSubLoop();

  while( !exitCode ) {
    exitCodeSub = false;
    test_RES();

    exitCodeSub = false;
    test_DIODE();

    exitCodeSub = false;
    test_CAP();

    exitCodeSub = false;
    test_TR();
    
//    exitCodeSub = false;
//    test_IND();
    
    mdelay( 200 );

  }
  
  clearPins();
  
  onExitSubLoop();
}
