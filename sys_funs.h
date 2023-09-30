// +------------------+
// | System functions |
// +------------------+


#define NUM_UNITS 7
#define ID_BASE_UNIT 2
char ct_units[] = { 'M', 'k', ' ', 'm', 'u', 'n', 'p' };


bool exitCode = false, exitCodeSub = false, modifyCode = false;

void onExitSubLoop() { exitCode = false; }
void onEnterSubLoop();
void onEnterSub_tester();
uint8_t pageCtr = 0, pageNum = 6;


#define F_BAT_MAX 4.1f
#define F_BAT_MIN 3.3f
float batVolt = 3.47f;
int batLvl = 74;

void checkBattery() {
	batLvl = 100*(batVolt - F_BAT_MIN)/ ( F_BAT_MAX - F_BAT_MIN );
}

void onEnterSubLoop() {
  exitCode = false;
  display.clearDisplay();
  display.setTextColor( WHITE, BLACK );  
  
  display.drawBitmap(119, 0, BMP_BAT_ICON, 8, 5, WHITE);
  uint8_t pBat = ( 5 * (100 - batLvl) )/100;
  display.fillRect( 121, 1, pBat, 3, BLACK ); // vertical cicrle
  display.display();
}

unsigned long _Tms = 0;
#define conDelay( Tm, stopCondition ) { _Tms = millis(); while( millis() - _Tms < Tm && !(stopCondition) ) delayMicroseconds( 900 ); }


void empty_loop() {
  onEnterSubLoop();

  display.setTextColor(1, 0); display.setTextSize(1);
  display.setCursor(55, 24);  display.print("Not");
  display.setCursor(31, 32);  display.print("Implemented");
  display.display();

  while ( !exitCode ) {
    exitCodeSub = false;
    conDelay( 1000, exitCode || exitCodeSub );
  }
  onExitSubLoop();
}


String fillPlacesLeft( String strIn, char charFill, uint8_t places ) {
  String strOut = strIn;
  while( strOut.length() < places ) strOut = charFill + strOut;
  return strOut;
}


template<typename T>
String val2str(T val, int baseSys = 2, uint8_t places = 0, char zeroChar = '0') {
  String ret = String(val, baseSys);
  while( ret.length() < places ) ret = zeroChar + ret;
  return ret;
}


void clearPins() {
  for( uint8_t i = 0; i < NUM_PIN_SUPPROT; i++ ) pinOff( pinsSupport[i] );
  for( uint8_t i = 0; i < 3; i++ ) pinOff( pins123[i] );
}

void onEnterSub_tester() {
  display.fillRect( 18, 0, 100, 8, BLACK );
  display.fillRect( 0, 7, 128, 57, BLACK );

  clearPins();
}



// ct_units
void printValue( float value, char c_unit, bool showDecs = true ) {
  String str = "", str_unit = "";
  float v2 = value;
  uint8_t ui1_unit_id = 2;

  while( v2 > 1000 && ui1_unit_id > 0 ) {
    v2 /= 1000;
    ui1_unit_id--;
  }
  
  while( v2 < 1 && ui1_unit_id < NUM_UNITS ) {
    v2 *= 1000;
    ui1_unit_id++;
  }
  
  if( ui1_unit_id == NUM_UNITS ) ui1_unit_id = ID_BASE_UNIT;
  
  str_unit = "[" + (String)( (char)ct_units[ ui1_unit_id ] ) + (String)c_unit + "]"; // contains unit, e.g.: "[uF]"
  str_unit.replace( " ", "" );

  int decLen = str_unit.length() < 4 ? 3 : 2;
  if( showDecs ) str = String( v2, decLen );
  else str = String( (int)round(v2) );
  
  if( str_unit.length() < 4 ) str_unit = " " + str_unit;
  
  if( v2 < 100 ) str = " " + str;
  if( v2 < 10 ) str = " " + str;

  if( !showDecs ) {
    for( int i = 0; i < decLen + 1; i++ ) str = " " + str;
  }
  
  display.setTextSize( 2 );
  
  // display unit first
  display.setCursor( 78, 16 );
  display.print( str_unit );
  
  // display value
  display.setCursor( 4, 16 );
  display.print( str );
}


void print_pin_numbers( uint8_t iA, uint8_t iB ) {
  display.setTextSize( 2 );
  display.setCursor( 26, 43 );
  display.print( iA < 3 ? String(iA + 1) : " " );

  display.setCursor( 88, 43 );
  display.print( iB < 3 ? String(iB + 1) : " "  );
}
