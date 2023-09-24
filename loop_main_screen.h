
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
//drawMenu( Menu, _selector('>'), _x(0), _y(0), _dY(0), elemens2Show(5),
//          state(SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT)[
//            HIDE_SELECTOR, SHOW_BIG_SELECT, SHOW_NUM_LINE, SHOW_NUM_LINE_SELECT
//          ] )
    drawMenu( mainMenu, 0, 4, 10, 2, 5, SHOW_NUM_LINE | SHOW_BIG_SELECT );
    display.display();
    conDelay( 1000, ( exitNow = btnEnc.wasPressed() ) || enc1.wasTurned() );
  }
  
  onExitSubLoop();
}
