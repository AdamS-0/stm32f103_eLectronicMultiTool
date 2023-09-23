// IR receiver/transmiter


//#define IR_RECEIVE_PIN          PA15 // PA6 PA15
//#define IR_SEND_PIN             PB9  // PA7
#define APPLICATION_PIN         PA2
#  if defined(ARDUINO_GENERIC_STM32F103C) || defined(ARDUINO_BLUEPILL_F103C8)
  // BluePill LED is active low
  #define FEEDBACK_LED_IS_ACTIVE_LOW
#  endif

#if !defined(STR_HELPER)
  #define STR_HELPER(x) #x
  #define STR(x) STR_HELPER(x)
#endif


/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols are active.
 */
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // the same as DECODE_KASEIKYO
//#define DECODE_LG
//#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER

//#define DECODE_DISTANCE     // universal decoder for pulse distance protocols
//#define DECODE_HASH         // special decoder for all protocols

#include <IRremote.hpp>

// Storage for the recorded code
struct IRDataStruct {
    IRData IRdata;
    uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
    uint8_t rawCodeLength; // The length of the code
} ;

#define NUM_IR_CODES 10
int16_t IRCodeId = 0, IRcodeSel = 0, listFirst = 0;
IRDataStruct IRcodes[NUM_IR_CODES];

void addCode(IRData *irdata);
void sendCode(uint16_t id);


void loop_IR_Rx() {
  if( exitCode ) return;
  onEnterSub_tester();

  display.setTextSize( 2 );
  display.setCursor( 4, 17 );
  display.print( "0x0000" );
  
  display.setTextSize( 1 );
  display.setCursor( 24, 0 ); display.print( "IR Rx |> Tx" );
  display.setCursor( 1, 37 ); display.print( "Protocol:" );
  display.setCursor( 1, 46 ); display.print( " Address:" );
  display.setCursor( 1, 55 ); display.print( "Raw data:" );
  display.display();

  IrReceiver.begin(PIN_IR_RECV, DISABLE_LED_FEEDBACK);
  //irrecv.enableIRIn();
    
  while( !exitCodeSub & !exitCode ) {
    
    if( btnEnc.wasPressed() ) {
      addCode(&IrReceiver.decodedIRData);
      exitCodeSub = true; // go to second page
    }
    
    if (IrReceiver.decode()) {
      display.fillRect( 0, 7, 128, 30, BLACK);
      display.fillRect( 55, 37, 73, 26, BLACK);
      display.setTextSize( 2 );
      display.setCursor( 4, 17 );
      display.print( "0x" );
      display.print( String( IrReceiver.decodedIRData.command, HEX ) );


      display.setTextSize( 1 );
      display.setCursor( 55, 37 );
      display.print( getProtocolString(IrReceiver.decodedIRData.protocol) );

      display.setCursor( 55, 46 );
      display.print( "0x" );
      display.print( String( IrReceiver.decodedIRData.address, HEX ) );

      display.setCursor( 55, 55 );
      display.print( "0x" );
      display.print( String( IrReceiver.decodedIRData.decodedRawData, HEX ) );
      
      IrReceiver.resume();
    }
    
    display.display();
    mdelay( 200 );
  }
  
}



void loop_IR_Tx() {
  if( exitCode ) return;
  enc1.getDelta();
  onEnterSub_tester();
  
  display.setTextSize( 1 );
  display.setCursor( 24, 0 ); display.print( "Rx <| IR Tx" );
  display.setCursor( 0, 10 ); display.print( "Protocol:" );
  display.setCursor( 0, 29 ); display.print( "Address:" );
  display.setCursor( 0, 48 ); display.print( "Raw Data:" );
  display.setCursor( 68, 8 ); display.print( "Code:  /" );
  display.setCursor( 116, 8 ); display.print( (String)NUM_IR_CODES );
  display.drawLine(61, 11, 61, 54, WHITE );
  display.display();

  IrSender.begin(PIN_IR_SEND, DISABLE_LED_FEEDBACK);
  //IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  int8_t IRcodeSel_last = 255;

  uint8_t y = 0, idList = 0;
  while( !exitCodeSub & !exitCode ) {
    
    IRcodeSel = IRcodeSel + enc1.getDelta();
    if( IRcodeSel < 0 ) IRcodeSel = 0;
    else if( IRcodeSel >= NUM_IR_CODES ) IRcodeSel = NUM_IR_CODES - 1;

    //    if( digitalRead( PIN_BTN_A ) == LOW )         sendCode( IRcodeSel );
    if( btnEnc.wasPressed() ) {
      sendCode( IRcodeSel );
    }
    
    if( IRcodeSel != IRcodeSel_last ) {
      IRcodeSel_last = IRcodeSel;
      display.fillRect( 0, 18, 61, 11, BLACK ); // clear protocol name
      display.fillRect( 0, 37, 61, 11, BLACK ); // clear address
      display.fillRect( 0, 56, 66, 8, BLACK ); // clear raw data

      display.setCursor( 0, 18 ); display.print( getProtocolString( IRcodes[IRcodeSel].IRdata.protocol ) );
      display.setCursor( 0, 37 ); display.print("0x"); display.print( String( IRcodes[IRcodeSel].IRdata.address, HEX ) );
      display.setCursor( 0, 56 ); display.print("0x"); display.print( String( IRcodes[IRcodeSel].IRdata.decodedRawData, HEX ) );
      
      display.drawLine(61, 11, 61, 54, WHITE );
      display.fillRect( 98, 8, 12, 8, BLACK ); // clear displayed id

      // display current id
      display.setCursor( 98, 8 );
      if( IRcodeSel < 9 ) display.print(" ");
      display.print( String(IRcodeSel + 1) );

      // draw list
      display.fillRect( 62, 16, 66, 48, BLACK ); // clear list


      // moving list range
      if( IRcodeSel > listFirst + 5 ) listFirst = IRcodeSel - 5;

      if( IRcodeSel < 0 ) IRcodeSel = 0;
      if( IRcodeSel < listFirst ) listFirst = IRcodeSel;

      if( listFirst + 5 > NUM_IR_CODES ) listFirst = NUM_IR_CODES - 6;
      
      // NUM_IR_CODES
      y = 16;
      idList = listFirst;
      for( uint8_t i = 0; i < 6; i++, y += 8, idList++ ) {
        display.setCursor( 68, y ); display.print( "0x" );
        if (IRcodes[idList].IRdata.protocol != UNKNOWN) display.print( String( IRcodes[idList].IRdata.command, HEX ) );
        else display.print( String( IRcodes[idList].IRdata.decodedRawData, HEX ) );
      }

      display.setCursor( 62, 16 + (IRcodeSel - listFirst)*8 );
      display.print(">");
    }
    
    display.display();
    conDelay( 200, exitCode || exitCodeSub || enc1.wasTurned() );
  }
  
}

// mdelay( 100, false ); // btnAinclude = 'false' to not skip mdelay



void loop_IR() {
  onEnterSubLoop();

  while( !exitCode ) {
    exitCodeSub = false;
    loop_IR_Rx();

    exitCodeSub = false;
    loop_IR_Tx();
    
    mdelay( 200 );
  }
  onExitSubLoop();
}















void addCode(IRData *irdata) {
  // IRCodeId
  IRcodes[IRcodeSel].IRdata = *irdata;

  if (IRcodes[IRcodeSel].IRdata.protocol == UNKNOWN) {
    IRcodes[IRcodeSel].rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
    IrReceiver.compensateAndStoreIRResultInArray(IRcodes[IRcodeSel].rawCode);
  } else {
    IRcodes[IRcodeSel].IRdata.flags = 0; // clear flags -esp. repeat- for later sending
  }

  //if( ++IRCodeId >= NUM_IR_CODES ) IRCodeId = 0;
}


void sendCode(uint16_t id) {
  if (IRcodes[id].IRdata.protocol == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    IrSender.sendRaw(IRcodes[id].rawCode, IRcodes[id].rawCodeLength, 38);
  } else {
    IrSender.write(&IRcodes[id].IRdata, 1);
  }
}


// decodedRawData
