///* return codes from endTransmission() */
//#define SUCCESS   0        /* transmission was successful */
//#define EDATA     1        /* too much data */
//#define ENACKADDR 2        /* received nack on transmit of address */
//#define ENACKTRNS 3        /* received nack on transmit of data */
//#define EOTHER    4        /* other error */
void loop_I2C_scanner() {
  onEnterSubLoop();

  clearPins();

  pinMode( PIN_1_R2, OUTPUT ); digitalWrite( PIN_1_R2, HIGH );
  pinMode( PIN_2_R2, OUTPUT ); digitalWrite( PIN_2_R2, HIGH );
  
  display.setTextSize( 1 );
  display.setCursor( 24, 0 ); display.print( "I2C Scanner" );
  display.setCursor( 0, 11 ); display.print( "1-SDA" );
  display.setCursor( 0, 21 ); display.print( "2-SCL" );
  display.setCursor( 0, 31 ); display.print( "3-GND" );
  display.setCursor( 0, 41 ); display.print( "Found" );
  display.display();

  byte error, address; //variable for error and I2C address
  int nDevices;
  
  // SCL, SDA
  // TwoWire WIRE3 (PIN_2, PIN_1, I2C_FAST_MODE);
  SoftWire WIRE3 (PIN_2, PIN_1, SOFT_FAST);
  WIRE3.begin();
  
  while( !exitCode ) {
    display.fillRect( 30, 10, 98, 54, BLACK ); // vertical cicrle
    
    String adr = "";
    nDevices = 0;
    for (address = 0; address < 127 && !exitCode; address++ ) {
      WIRE3.beginTransmission(address);
      error = WIRE3.endTransmission();
      if (error == 0) {
        adr = "0x";
        if( address < 16 ) adr += "0";
        adr += String(address, HEX);
        if( nDevices < 18 ) {
          display.setCursor( 40 + (nDevices/6)*30, 11 + (nDevices % 6)*9 ); display.print( adr );
        }
        nDevices++;
      }
    }

    // if there were errors, aka fake slaves (more then half max = 64) erase them
    if( nDevices >= 64 ){
      display.fillRect( 40, 11, 84, 53, BLACK);
      nDevices = 0;
    }
    
    display.fillRect( 0, 51, 30, 7, BLACK);
    display.setCursor( 0, 51 ); display.print( (String)nDevices );
    
    display.display();
    conDelay( 200, exitCode );
  }
  WIRE3.end();
  
  onExitSubLoop();
}
