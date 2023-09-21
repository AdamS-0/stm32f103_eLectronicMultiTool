// +---------------------+
// | Analog Read Section |
// +---------------------+
#define TIME_AREAD_DELAY  200
float mes_V = 0, mes_C = 0; // [V], [mA]

void measure_voltage() {
  if( exitCode ) return;
  onEnterSub_tester();
  display.setTextSize( 2 );
  display.setCursor( 90, 16 );
  display.print( "[V]" );
  display.display();
  
  while( !exitCodeSub & !exitCode ) {
    display.setCursor( 4, 16 );
    if( mes_V < 100 ) display.write( 0x20 );
    if( mes_V < 10 ) display.write( 0x20 );
    display.print( String( mes_V, 3 ) );
    display.display();
    mdelay( 1000 );
  }
}

void measure_current() {
  if( exitCode ) return;
  onEnterSub_tester();
  display.setTextSize( 2 );
  display.setCursor( 78, 16 );
  display.print( "[mA]" );
  display.display();

  while( !exitCodeSub & !exitCode ) {
    display.setCursor( 4, 16 );
    if( mes_C < 1000 ) display.write( 0x20 );
    if( mes_C < 100 ) display.write( 0x20 );
    if( mes_C < 10 ) display.write( 0x20 );
    display.print( String( mes_C, 1 ) );display.display();
    mdelay( 1000 );
  }
}


// analog input
void loop_multimeter() {
  onEnterSubLoop();

  while( !exitCode ) {
    exitCodeSub = false;
    measure_voltage();

    exitCodeSub = false;
    measure_current();
    
    mdelay( 200 );
  }
  onExitSubLoop();
}
