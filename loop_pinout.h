
void drawPinout();

void loop_pinout() {
  onEnterSubLoop();
  drawPinout();
  conDelay( 10000, exitCode || btnA.wasPressed() || btnB.wasPressed() || btnEnc.wasPressed() );
  onExitSubLoop();
}


void drawPinout() {
  display.clearDisplay();
  
  uint8_t x = 8, dx = 13;
  for( uint8_t xi = 0; xi < 9; xi++ ) {
    display.drawRect(x, 30, 3, 5, 1);
    display.drawRect(x, 45, 3, 5, 1);
    x += dx;
  }


  // conncted 1, 2, 3, PWM_OUT
  x = 22;
  for( uint8_t xi = 0; xi < 4; xi++ ) {
    display.drawFastVLine(x, 35, 9, 1);
    x += dx;
  }

  // PWM line hor
  display.drawFastVLine(61, 17, 11, 1);

  // 3.3v line hor
  display.drawFastVLine(9, 10, 18, 1);

  display.drawFastHLine(26, 3, 48, 1);
  display.drawFastVLine(74, 4, 24, 1);

  // SPI-SCK
  display.drawFastHLine(87, 3, 16, 1);
  display.drawFastVLine(87, 4, 24, 1);

  // SPI-MISO
  display.drawFastHLine(100, 12, 3, 1);
  display.drawFastVLine(100, 13, 15, 1);

  // SPI-MOSI
  display.drawFastVLine(113, 26, 2, 1);


  // GND
  display.drawFastVLine(113, 51, 3, 1);
  display.drawFastHLine(108, 55, 10, 1);
  display.drawFastHLine(110, 58, 6, 1);
  display.drawFastHLine(112, 61, 2, 1);

  display.setTextSize(1);
  display.setCursor(0, 0); display.print("3.3V");
  display.setCursor(52, 8); display.print("PWM");
  display.setCursor(19, 19); display.print("1");
  display.setCursor(32, 19); display.print("2");
  display.setCursor(45, 19); display.print("3");
  display.setCursor(3, 53); display.print("5V");
  display.setCursor(19, 53); display.print("1");
  display.setCursor(32, 53); display.print("2");
  display.setCursor(45, 53); display.print("3");
  display.setCursor(65, 53); display.print("RST");
  display.setCursor(104, 0); display.print("SCK");
  display.setCursor(104, 9); display.print("MISO");
  display.setCursor(104, 18); display.print("MOSI");
  display.display();
}
