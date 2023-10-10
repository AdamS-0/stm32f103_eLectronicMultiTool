
//   1 - R
//   2 - Common (R1 = 470 Ohm)
//   3 - G
// PWM - B

#define RGB_COM_ANODE   1
#define RGB_COM_CATHODE 0

#define RGB_PIN_R       PIN_1
#define RGB_PIN_G       PIN_3
#define RGB_PIN_B       PIN_PWM_OUT
#define RGB_PIN_COMMON  PIN_2  // PIN_2_R1

// y position for: Common, R, G, B
uint8_t RGB_Y[] = { 40, 12, 30, 48 };

void RGB_PIN_OUT(bool RGBCommon, uint32_t RGB_PIN, uint8_t RGB_val, uint8_t &RGB_valPrev) {
  if( RGB_val != RGB_valPrev ) {
    RGB_valPrev = RGB_val;
    analogWrite( RGB_PIN, RGB_val );
  }
}




#define NUM_RGB_OPTS_PAGES 4
String strRGB[] = { "C", "R", "G", "B" };
Menu menuRGB( strRGB, NUM_RGB_OPTS_PAGES );

#define NUM_RGB_COMMON 2
String strRGBCommon[] = { "Anode", "Cathode" };
Menu menuRGBCommon( strRGBCommon, NUM_RGB_COMMON );


void RGB_showMenuSelector( char cSel = '>' ) {
  uint8_t _x = 0, _y = 40; // Common: menuRGB.id == 0
  display.fillRect( 0, 40, 7, 8, BLACK ); // Common
  display.fillRect( 56, 12, 12, 52, BLACK ); // RGB
  
  display.setTextSize(1);
  if( menuRGB.id > 0 ) { // RGB
    display.setTextSize(2);
    _x = 56;
    _y = RGB_Y[ menuRGB.id ];
  }
  display.setCursor(_x, _y);
  display.print((String)cSel);
}

void loop_RGB_LED() {
  onEnterSubLoop();
  
  bool bMenuChanging = true, bMenuChanging_change = false, bBtnB_pressed = false, bBtnEnc_pressed = false;
  bool RGBCommon = RGB_COM_CATHODE, RGBCommonPrev = RGB_COM_CATHODE;
  uint8_t valR = 0, valRPrev = 1;
  uint8_t valG = 0, valGPrev = 1;
  uint8_t valB = 0, valBPrev = 1;
  uint8_t val0 = 0;
  uint16_t conDelayTime = 1000;
  menuRGBCommon.id = 1;
  uint8_t* RGB_Vals[] = { &val0, &valR, &valG, &valB };

  pinMode( RGB_PIN_R, OUTPUT ); pinMode( RGB_PIN_G, OUTPUT );
  pinMode( RGB_PIN_B, OUTPUT ); pinMode( RGB_PIN_COMMON, OUTPUT );

  digitalWrite( RGB_PIN_COMMON, RGBCommon );
  
  display.setTextColor(1, 0); display.setTextSize(1);
  display.setCursor(68, 0);   display.print("RGB LED");
  display.setCursor(0, 0);    display.print("1-R");
  display.setCursor(0, 8);    display.print("2-Common");
  display.setCursor(0, 16);   display.print("3-G");
  display.setCursor(0, 24);   display.print("PWM-B");

  display.setCursor(6, 40);   display.print("Common:");
  display.setCursor(12, 48);  display.print("Anode");
  display.setCursor(12, 56);  display.print("Cathode");

  display.setTextSize(2);
  display.setCursor(68, 12);  display.print("R:");
  display.setCursor(68, 30);  display.print("G:");
  display.setCursor(68, 48);  display.print("B:");

  display.setTextSize(2);
  for( uint8_t i = 1; i < 4; i++ ) {
    display.setCursor( 92, RGB_Y[ i ] );  display.print( val2str( *RGB_Vals[i], 10, 3, '0') );
  }

  menuRGBCommon.draw( '>', 6, 48, 0, 2, SHOW_ONLY_SELECTOR );
  
  display.display();

  while ( !exitCode ) {
    exitCodeSub = false;

    // step into editing mode, by pressing encoder button
    if( bBtnEnc_pressed ) {
      bBtnEnc_pressed = false;
      bMenuChanging_change = bMenuChanging || menuRGB.id == 0;
    } else if( bBtnB_pressed ) {
      bBtnB_pressed = false;
      bMenuChanging_change = !bMenuChanging;
    }
    
    if( bMenuChanging_change ) {
      bMenuChanging_change = false;
      bMenuChanging = !bMenuChanging;

      if( bMenuChanging ) {
        display.fillRect( 92, 12, 36, 52, BLACK ); // RGB values
        display.setTextSize(2);
        for( uint8_t i = 1; i < 4; i++ ) {
          display.setCursor( 92, RGB_Y[ i ] );  display.print( val2str( *RGB_Vals[i], 10, 3, '0') );
        }
      } else {
        RGB_showMenuSelector('?');
      }
    }
    
    if( bMenuChanging ) {
      menuRGB.update( enc1.getDelta() );
      encoderChangeClear();
      RGB_showMenuSelector( '>' );
    } else {
      if( menuRGB.id == 0 ) {
        menuRGBCommon.update( enc1.getDelta() );
        menuRGBCommon.draw( '>', 6, 48, 0, 2, SHOW_ONLY_SELECTOR );
        RGBCommon = menuRGBCommon.id == 0 ? RGB_COM_ANODE : RGB_COM_CATHODE;

      } else if( menuRGB.id > 0 ) {
        encoderChangeValue( *RGB_Vals[ menuRGB.id ], 10, (uint8_t)255 );
        encoderBlinkValue( 92, RGB_Y[ menuRGB.id ], *RGB_Vals[ menuRGB.id ], 2, 10, 3 );
      }
    }
    
    // update pins
    if( RGBCommon != RGBCommonPrev && !bMenuChanging ) {
      RGBCommonPrev = RGBCommon;
      display.setTextSize(2);
      for( uint8_t i = 1; i < 4; i++ ) {
        *RGB_Vals[i] = 255 - *RGB_Vals[i];
        display.setCursor( 92, RGB_Y[ i ] );  display.print( val2str( *RGB_Vals[i], 10, 3, '0') );
      }
      digitalWrite( RGB_PIN_COMMON, RGBCommon );
    }

    RGB_PIN_OUT( RGBCommon, RGB_PIN_R, valR, valRPrev );
    RGB_PIN_OUT( RGBCommon, RGB_PIN_G, valG, valGPrev );
    RGB_PIN_OUT( RGBCommon, RGB_PIN_B, valB, valBPrev );
    
    display.display();

    conDelayTime = bMenuChanging || b_encChangePlace ? 1000 : 200;
    conDelay( conDelayTime, exitCode || enc1.wasTurned() || ( bBtnB_pressed = btnB.wasPressed() ) || ( bBtnEnc_pressed = btnEnc.wasPressed(bMenuChanging || ( !bMenuChanging && menuRGB.id == 0) ) ) );
    
  }

  clearPins();
  onExitSubLoop();
}
