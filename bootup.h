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
  
  display.print( "OK.\nFilters..." ); display.display();
  
  err = fiR.init();
  if( err == 0 ) display.print("OK.");
  else display.print("FAIL.");
  delay( 50 );
  
  display.display();
}
