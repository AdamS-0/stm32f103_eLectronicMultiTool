// +----------------------+
// | Servo Tester Section |
// +----------------------+

// us: 500, 2200
void loop_servo() {
  onEnterSubLoop();
  encoderChangeClear();
  uint16_t lastPos = 0, pos = 1350, da = 1; // [us]
  
  display.setTextSize( 1 );
  display.setCursor( 28, 0 ); display.print( "Servo tester" );
  
  display.setTextSize( 2 );
  display.setCursor(64, 20);
  display.print("[us]");
  
  lastPos = 0;
  myservo.attach(PIN_PWM_OUT);
  while( !exitCode ) {
    
    encoderChangeValue(pos, 10);
    
    if( pos != lastPos ) {
      myservo.writeMicroseconds( pos );
      lastPos = pos;
    }
    encoderBlinkValue( 16, 20, pos, 2, 10, 4 ); // x, y, val, textSize, base, places
    display.display();
    conDelay( 200, exitCode || enc1.wasTurned() );
  }
  myservo.detach();
  onExitSubLoop();
}





//void loop_PWM_gen() {
//  onEnterSubLoop();
//  uint8_t prevValue = 1, currValue = 0;
//  display.setTextSize( 1 );
//  display.setCursor( 22, 0 ); display.print( "PWM generator" );
//  display.setTextSize( 2 );
//  display.setCursor( 2, 12 ); display.print( "duty" );
//  display.setCursor( 2, 32 ); display.print( "freq" );
//  display.setCursor( 75, 32 ); display.print( "---" );
//  display.display();
//  
//  pinMode( PIN_PWM_OUT, OUTPUT );
//  analogWrite( PIN_PWM_OUT, currValue );
//  
//  while( !exitCode ) {
//    encoderChangeValue(currValue, 10);
//    if( prevValue != currValue ) {
//      prevValue = currValue;
//      analogWrite( PIN_PWM_OUT, currValue );
//      
////      str_curVal = val2str(currValue, 10, 3, '0');
////      display.setCursor( 75, 12 );
////      display.print( str_curVal );
//    }
//    encoderBlinkValue( 75, 12, currValue, 2, 10, 3 ); // x, y, val, textSize, base, places
//    display.display();
//    
//    conDelay( 200, exitCode || enc1.wasTurned() );
//  }
//  currValue = 0;
//  analogWrite( PIN_PWM_OUT, currValue );
//  pinMode( PIN_PWM_OUT, INPUT );
//  
//  mdelay( 200 );
//  onExitSubLoop();
//}
