// continuity
void loop_continuity() {
  onEnterSubLoop();

  display.setTextColor( WHITE, BLACK );
  display.setTextSize( 1 );
  display.setCursor( 24, 0 ); display.print( "Continuity" );
  display.setTextSize( 2 );
  
  display.drawBitmap(90, 25, BMP_SIGNAL, 25, 21, WHITE);
  display.display();
  
  bool P12 = false, P13 = false, P23 = false;
  
  while( !exitCode ) {

    display.fillRect(17, 13, 54, 44, BLACK);
    display.setCursor( 38, 13 ); display.print( "1" );
    display.setCursor( 17, 41 ); display.print( "2" );
    display.setCursor( 59, 41 ); display.print( "3" );

    clearPins();

    // check 1-2 & 1-3
    pinMode( PIN_1, OUTPUT );
    pinMode( PIN_2, INPUT_PULLDOWN ); pinMode( PIN_3, INPUT_PULLDOWN );
    digitalWrite( PIN_1, HIGH );

    P12 = digitalRead( PIN_2 );
    P13 = digitalRead( PIN_3 );

    delay(5);
    
    pinMode( PIN_1, INPUT );
    pinMode( PIN_2, OUTPUT );
    digitalWrite( PIN_2, HIGH );
    P23 = digitalRead( PIN_3 );
    
    if( P12 ) display.drawLine(37, 21, 25, 38, WHITE);
    if( P13 ) display.drawLine(51, 21, 64, 38, WHITE);
    if( P23 ) display.fillRect(31, 47, 28, 2, WHITE);
    
    display.display();
    mdelay( 200 );
  }
  
  clearPins();
  onExitSubLoop();
}
