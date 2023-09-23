void bootup() {
  display.setFont( SmallFont );
  display.setBrightness( 0 );
  
  display.clearDisplay();
  display.setTextColor( WHITE, BLACK );

  display.setTextSize( 1 );
  display.setCursor( 0, 0 );
  display.print( "Screen... " ); uint8_t err = display.display();
  delay( 50 );
  
  if( err == 0 ) display.print("OK.");
  else display.print("FAIL.");
  display.display();
  display.print( "\nGPIO's... " ); display.display();
  
  //pinMode( PIN_LIGHT_SENS, INPUT );
  
  pinMode( PIN_ENC_A, INPUT_PULLUP );
  pinMode( PIN_ENC_B, INPUT_PULLUP );
  
  delay( 50 );
  display.print( "OK.\nInterrupts... " ); display.display();

  attachInterrupt( digitalPinToInterrupt( enc1.pinCLK ), INT_PIN_ENC_A, CHANGE );
  attachInterrupt( digitalPinToInterrupt( enc1.pinDT ), INT_PIN_ENC_B, CHANGE );
  
  btns_bootup();
//  attachInterrupt( digitalPinToInterrupt( PIN_ENC_BTN ), int_encBtn, FALLING );
//  
//  attachInterrupt( digitalPinToInterrupt( PIN_BTN_A ), int_btn_A, FALLING );
//  attachInterrupt( digitalPinToInterrupt( PIN_BTN_B ), int_btn_B, FALLING );
  
  /*
  delay( 50 );
  display.print( "OK.\nDHT11... " ); display.display();
  
  dht.begin();
  
  delay( 50 );
  unsigned long tDHT11 = millis();
  while( isnan( dht_humid_old = dht.readHumidity() ) && millis() - tDHT11 < 500 ) delay( 100 );
//  while( isnan( dht_temp_old = dht.readTemperature() + DHT_TEMP_ADD ) && millis() - tDHT11 < 500 ) delay( 100 );
  dht_temp_old = dht.readTemperature() + DHT_TEMP_ADD;
  if( isnan( dht_humid_old ) || isnan( dht_temp_old ) ) {
          display.print( " Missing" );
  } else  display.print( " OK" );
  */
  
  display.print( "OK.\nFilters..." ); display.display();
  
  err = fiR.init();
  if( err == 0 ) display.print("OK.");
  else display.print("FAIL.");
  delay( 50 );

  // initializing menus
  mainMenu.names = strMenuPages;
  mainMenu.Len = ArraySize( strMenuPages );
  
  display.display();
}
