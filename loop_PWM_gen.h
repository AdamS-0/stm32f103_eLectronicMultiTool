// +-----------------------+
// | PWM Generator Section |
// +-----------------------+

void loop_PWM_gen() {
  onEnterSubLoop();
  encoderChangeClear();
  uint8_t prevValue = 1, currValue = 0;
  display.setTextSize( 1 );
  display.setCursor( 22, 0 ); display.print( "PWM generator" );
  display.setTextSize( 2 );
  display.setCursor( 2, 12 ); display.print( "duty" );
  display.setCursor( 2, 32 ); display.print( "freq" );
  display.setCursor( 75, 32 ); display.print( "---" );
  display.display();
  
  pinMode( PIN_PWM_OUT, OUTPUT );
  analogWrite( PIN_PWM_OUT, currValue );
  
  while( !exitCode ) {
    encoderChangeValue(currValue, 10);
    if( prevValue != currValue ) {
      prevValue = currValue;
      analogWrite( PIN_PWM_OUT, currValue );
      
//      str_curVal = val2str(currValue, 10, 3, '0');
//      display.setCursor( 75, 12 );
//      display.print( str_curVal );
    }
    encoderBlinkValue( 75, 12, currValue, 2, 10, 3 ); // x, y, val, textSize, base, places
    display.display();
    
    conDelay( 200, exitCode || enc1.wasTurned() );
  }
  currValue = 0;
  analogWrite( PIN_PWM_OUT, currValue );
  pinMode( PIN_PWM_OUT, INPUT );
  
  conDelay( 200, exitCode );
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
