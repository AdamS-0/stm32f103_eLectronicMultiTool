// +----------------------+
// | Piezo tester Section |
// +----------------------+

void loop_piezo() {
  onEnterSubLoop();
  encoderChangeClear();
  uint16_t prevTone = 1, currTone = 1000; // Hz
  bool playTone = false, playToneChanged = false, toneChanged = false;
  display.setTextSize( 1 );
  display.setCursor( 28, 0 ); display.print( "Piezo tester" );

  display.setCursor( 0, 11 ); display.print( "Frequency:" );
  display.setCursor( 0, 45 ); display.print( "Play (B):" + (String)(playTone ? "On" : "Off") );
  display.setTextSize( 2 );
  display.setCursor( 70, 23 ); display.print( "[Hz]" );
  display.display();
  
  pinMode( PIN_PWM_OUT, OUTPUT );
  clearPins();
  for( uint8_t i = 0; i < 3; i++ ) {
    pinMode( pinsSupportLowR[i], OUTPUT );
    digitalWrite( pinsSupportLowR[i], LOW );
  }
  
  while( !exitCode ) {
    if( playToneChanged ) {
      playToneChanged = false;
      playTone = !playTone;
      toneChanged = true;
      display.setTextSize(1);
      display.setCursor( 54, 45 ); display.print( playTone ? "On " : "Off" );
    }
    
    encoderChangeValue(currTone, 10);
    if( prevTone != currTone ) {
      toneChanged = true;
      prevTone = currTone;
    }

    if( toneChanged ){
      toneChanged = false;
      if(playTone)  tone( PIN_PWM_OUT, currTone );
      else          noTone( PIN_PWM_OUT );
    }
    encoderBlinkValue( 10, 23, currTone, 2, 10, 5 ); // x, y, val, textSize, base, places
    display.display();
    
    conDelay( 200, exitCode || ( playToneChanged = btnB.wasPressed() ) || enc1.wasTurned() );
  }
  currTone = 0;
  noTone( PIN_PWM_OUT );
  clearPins();
  
  onExitSubLoop();
}





//void loop_PWM_gen() {
//  onEnterSubLoop();
//
//  while( !exitCode ) {
//    exitCodeSub = false;
//    loop_PWM_one();
//
//    exitCodeSub = false;
//    servo_tester();
//    
//    mdelay( 200 );
//  }
//  onExitSubLoop();
//}
