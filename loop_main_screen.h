
void loop_mainScreen() {
  bool exitNow = false;
  
  enc1.getDelta(); // erase turns from previous screens
  onEnterSubLoop();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print( projName );
  display.display();
  
  while( !exitNow ) {
    exitCodeSub = false;
    
    updateMenu( mainMenu, enc1.getDelta() );
    display.fillRect( 0, 8, 128, 56, BLACK );
    //drawMenu( Menu, _x(0), _y(0), _dY(0), elemens2Show(5), showBigSelected(false) )
    drawMenu( mainMenu, 0, 8, 10, 2, 5, true );
    display.display();
    conDelay( 1000, ( exitNow = btnEnc.wasPressed() ) || enc1.wasTurned() );
  }
  
  onExitSubLoop();
}
