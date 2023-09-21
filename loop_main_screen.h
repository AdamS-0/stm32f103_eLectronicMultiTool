
void loop_mainScreen() {
  int8_t minId = 0, maxId = NUM_MENU_PAGES, countOnScr = 5;
  uint8_t curId = 0, my = 0;
  
  enc1.getDelta(); // erase turns from previous screens
  onEnterSubLoop();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print( projName );
  display.display();
  
  while( !exitCode ) {
    exitCodeSub = false;

    display.fillRect( 0, 8, 128, 56, BLACK );
    
    selMenu = (selMenu + enc1.getDelta() );
    while( selMenu < 0 ) selMenu += NUM_MENU_PAGES;
    selMenu = selMenu % NUM_MENU_PAGES;
    minId = max(selMenu - 2, 0);
    maxId = min( minId + countOnScr, NUM_MENU_PAGES );
    minId = min( (int)minId, maxId - countOnScr );
    minId = minId < 0 ? 0 : minId;
    
    my = 10;
    for( uint8_t i = minId; i < maxId; i++ ) {
      curId = selMenu == i;
      String str = strMenuPages[i];
      str = str.substring( 0, min( (curId ? 10 : 17), (int)str.length() ) );
      display.setTextSize( 1 + curId );
      display.setCursor( 8 - 8*curId, my );
      display.print( ( curId ? "" : String(i + 1) + " " ) + str );
      my += 10 + curId*6;
    }
    
    display.display();
    
    conDelay( 1000, exitCode || exitCodeSub || enc1.wasTurned() );
  }
  
  onExitSubLoop();
}
